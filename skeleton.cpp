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
    m_bones.clear();
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
            if (boneIndex > m_bones.size())
                m_bones.resize(boneIndex);
            m_bones[boneIndex - 1].push_back({iVertex, jsonWeights[iRow].toDouble()});
        }
    }

    return true;
}


bool CSkeleton::loadTransforms(const QString &inverseFilename, const QString &newFilename)
{
    return loadTransform(inverseFilename, m_inverseTransform) &&
            loadTransform(newFilename, m_newTransform);
}


bool CSkeleton::loadTransform(const QString &filename, std::vector<TTransformMatrix> &transform)
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
            transform[iBone].values[iValue] = boneTransform[iValue].toDouble();
    }

    return true;
}
