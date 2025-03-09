#include "skeletalTransform.hpp"

CSkeletalTransform::CSkeletalTransform(CSkeletalMesh &skeletalMesh) :
    m_skeletalMesh(skeletalMesh)
{
}


bool CSkeletalTransform::loadTransforms(const QString &inverseFilename, const QString &newFilename)
{
    std::vector<mutil::Matrix4> newTransform;
    std::vector<mutil::Matrix4> inverseTransform;
    if (!loadTransform(inverseFilename, newTransform))
        return false;
    if (!loadTransform(newFilename, inverseTransform))
        return false;
    if (newTransform.size() != inverseTransform.size())
        return false;

    // Create multiplied transforms
    m_transforms.resize(newTransform.size());
    for (quint32 iTransform = 0; iTransform < m_transforms.size(); iTransform++)
        m_transforms[iTransform] = inverseTransform[iTransform] * newTransform[iTransform];

    return true;
}


void CSkeletalTransform::applyTransforms()
{
    std::vector<CSkeletalMesh::FVertex> &vertices = m_skeletalMesh.getVertices();
    for (quint32 iVertex = 0; iVertex < vertices.size(); iVertex++)
    {
        // Convert vertex position to homogeneous coordinates
        mutil::Vector4 originVertex;
        originVertex.vec[0] = vertices[iVertex].position.X;
        originVertex.vec[1] = vertices[iVertex].position.Y;
        originVertex.vec[2] = vertices[iVertex].position.Z;
        originVertex.vec[3] = 1.0f;
        mutil::Vector4 transformedVertex(0.0f, 0.0f, 0.0f, 1.0f);

        // Apply all bones transforms
        for (qint32 iRow = 0; iRow < CSkeletalMesh::VERTEX_BONES_MAX; iRow++)
            if (vertices[iVertex].weights[iRow].first > 0)
                transformedVertex += m_transforms[vertices[iVertex].weights[iRow].first] * originVertex * vertices[iVertex].weights[iRow].second;

        // Save updated vertex position
        // TODO: Maybe should be divided by vertices[iVertex].W
        vertices[iVertex].position.X = transformedVertex.vec[0];
        vertices[iVertex].position.Y = transformedVertex.vec[1];
        vertices[iVertex].position.Z = transformedVertex.vec[2];
    }
}


bool CSkeletalTransform::loadTransform(const QString &filename, std::vector<mutil::Matrix4> &transform)
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
