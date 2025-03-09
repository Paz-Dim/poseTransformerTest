#include "skeleton.hpp"

CSkeleton::CSkeleton(CMeshProcessor &meshProcessor) :
    m_meshProcessor(meshProcessor)
{
}


bool CSkeleton::loadSkeleton(const QString &filename)
{
    // Open skeleton file
    QFile skeletonFile;
    skeletonFile.setFileName(filename);
    skeletonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!skeletonFile.isOpen())
        return false;

    // Read text
    QString skeletonText = skeletonFile.readAll();
    skeletonFile.close();
    // Convert to JSON document
    QJsonDocument skeletonJson = QJsonDocument::fromJson(skeletonText.toUtf8());
    QJsonArray rootArray = skeletonJson.array();

    // Check number of items
    if (static_cast<quint32>(rootArray.size()) != m_meshProcessor.getVertices().size())
        return false;

    // Process array
    m_verticesBones.resize(rootArray.size());
    for (qint32 iVertex = 0; iVertex < rootArray.size(); iVertex++)
    {
        if (!rootArray[iVertex].isObject())
            return false;
        // Get weights and indices fields
        QJsonArray jsonWeights = rootArray[iVertex].toObject().value("weight").toArray();
        QJsonArray jsonIndices = rootArray[iVertex].toObject().value("index").toArray();
        if ((jsonWeights.size() != 4) || (jsonIndices.size() != 4))
            return false;

        // Convert JSON data to skeleton and store it
        for (qint32 iRow = 0; iRow < 4; iRow++)
        {
            // Bone index, beginning from 1
            quint32 boneIndex = jsonIndices[iRow].toInt();
            if (boneIndex == 0)
                break;
            m_verticesBones[iVertex].push_back({boneIndex - 1, jsonWeights[iRow].toDouble()});
        }
    }

    return true;
}


bool CSkeleton::loadTransforms(const QString &inverseFilename, const QString &newFilename)
{
    std::vector<mutil::Matrix4> newTransform;
    std::vector<mutil::Matrix4> inversedTransform;
    if (!loadTransform(inverseFilename, newTransform))
        return false;
    if (!loadTransform(newFilename, inversedTransform))
        return false;
    if (newTransform.size() != inversedTransform.size())
        return false;

    // Create multiplied transforms
    m_transforms.resize(newTransform.size());
    for (quint32 iTransform = 0; iTransform < m_transforms.size(); iTransform++)
        m_transforms[iTransform] = inversedTransform[iTransform] * newTransform[iTransform];

    return true;
}


void CSkeleton::applyTransforms()
{
    std::vector<CMeshProcessor::FVector> &vertices = m_meshProcessor.getVertices();
    for (quint32 iVertex = 0; iVertex < vertices.size(); iVertex++)
    {
        mutil::Vector4 originVertex;
        originVertex.vec[0] = vertices[iVertex].X;
        originVertex.vec[1] = vertices[iVertex].Y;
        originVertex.vec[2] = vertices[iVertex].Z;
        originVertex.vec[3] = 1.0f;
        mutil::Vector4 transformedVertex(0.0f, 0.0f, 0.0f, 1.0f);

        // Apply all bones transforms
        for (const std::pair<qint32, float> &bone : m_verticesBones[iVertex])
            transformedVertex += m_transforms[bone.first + 1] * originVertex * bone.second;

        // TODO: Maybe should be divided by vertices[iVertex].W
        vertices[iVertex].X = transformedVertex.vec[0];
        vertices[iVertex].Y = transformedVertex.vec[1];
        vertices[iVertex].Z = transformedVertex.vec[2];
    }
}


bool CSkeleton::loadTransform(const QString &filename, std::vector<mutil::Matrix4> &transform)
{
    // Open transform file
    QFile transformFile;
    transformFile.setFileName(filename);
    transformFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!transformFile.isOpen())
        return false;

    // Read text
    QString transformText = transformFile.readAll();
    transformFile.close();
    // Convert to JSON document
    QJsonDocument skeletonJson = QJsonDocument::fromJson(transformText.toUtf8());
    QJsonArray rootArray = skeletonJson.array();

    // Save bones transforms
    transform.resize(rootArray.size());
    for (qint32 iBone = 0; iBone < rootArray.size(); iBone++)
    {
        if (!rootArray[iBone].isArray())
            return false;
        QJsonArray boneTransform = rootArray[iBone].toArray();
        if (boneTransform.size() != TRANSFORM_MATRIX_LENGTH)
            return false;
        // Save bone values
        for (qint32 iValue = 0; iValue < TRANSFORM_MATRIX_LENGTH; iValue++)
            transform[iBone].mat[iValue] = boneTransform[iValue].toDouble();
    }

    return true;
}
