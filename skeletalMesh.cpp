#include "skeletalMesh.hpp"

bool CSkeletalMesh::load(const QString &meshFilename, const QString &skeletonFilename)
{
    // Remove old data
    m_mesh.vertices.clear();
    m_mesh.indices.clear();

    if (!loadMesh(meshFilename))
        return false;
    if (!loadSkeleton(skeletonFilename))
        return false;

    return true;
}


bool CSkeletalMesh::saveMesh(const QString &filename)
{
    // Positions
    m_posCounter = 0;
    auto posMapper = [this]()
    {
        // No vertices
        if (m_posCounter == m_mesh.vertices.size())
            return thinks::ObjEnd<TPos>();

        // Map indicates that additional positions may be available after this one
        auto result = thinks::ObjMap(TPos(m_mesh.vertices[m_posCounter].position.X,
                                          m_mesh.vertices[m_posCounter].position.Y,
                                          m_mesh.vertices[m_posCounter].position.Z));
        m_posCounter++;
        return result;
    };

    // Faces
    quint32 indexCounter = 0;
    auto faceMapper = [this, &indexCounter]()
    {
        // Check that there are 3 more indices (trailing indices handled below)
        if ((m_mesh.indices.size() - indexCounter) < 3)
            return thinks::ObjEnd<TFace>();

        // Create a face from the mesh indices.
        auto result = thinks::ObjMap(TFace{TObjIndex(m_mesh.indices[indexCounter]),
                                           TObjIndex(m_mesh.indices[indexCounter + 1]),
                                           TObjIndex(m_mesh.indices[indexCounter + 2])});
        indexCounter += 3;
        return result;
    };

    /* Open the OBJ file and pass in the mappers, which will be called
     * internally to write the contents of the mesh to the file */
    auto ofs = std::ofstream(filename.toStdString());
    if (!ofs)
        return false;
    const auto result = thinks::WriteObj(ofs, posMapper, faceMapper);
    ofs.close();

    // Some sanity checks
    if (result.position_count != m_mesh.vertices.size())
        return false;
    if (result.face_count != m_mesh.indices.size() / 3)
        return false;
    if (indexCounter != m_mesh.indices.size())
        return false;

    return true;
}


std::vector<CSkeletalMesh::FVertex> &CSkeletalMesh::getVertices()
{
    return m_mesh.vertices;
}


bool CSkeletalMesh::loadMesh(const QString &filename)
{
    // Reset counter
    m_posCounter = 0;

    // Positions
    auto addPosition = thinks::MakeObjAddFunc<TPos>([this](const TPos &pos)
    {
        readPosCallback(pos);
    });
    // Faces
    auto addFace = thinks::MakeObjAddFunc<TFace>([this](const TFace &face)
    {
        readFaceCallback(face);
    });

    // Open the OBJ file and populate the mesh while parsing it
    auto ifs = std::ifstream(filename.toStdString());
    if (!ifs)
        return false;
    const auto result = thinks::ReadObj(ifs, addPosition, addFace);
    ifs.close();

    // Some sanity checks
    if (result.position_count != m_mesh.vertices.size())
        return false;

    return true;
}


void CSkeletalMesh::readPosCallback(const TPos &pos)
{
    // Check if we need a new vertex
    if (m_mesh.vertices.size() <= m_posCounter)
        m_mesh.vertices.emplace_back();

    // Write the position property of current vertex
    m_mesh.vertices[m_posCounter].position = {pos.values[0], pos.values[1], pos.values[2]};
    m_posCounter++;
}


void CSkeletalMesh::readFaceCallback(const TFace &face)
{
    // Add triangle indices into the linear storage of our mesh class
    m_mesh.indices.push_back(face.values[0].value);
    m_mesh.indices.push_back(face.values[1].value);
    m_mesh.indices.push_back(face.values[2].value);
}


bool CSkeletalMesh::loadSkeleton(const QString &filename)
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
    if (static_cast<quint32>(rootArray.size()) != m_mesh.vertices.size())
        return false;

    // Process array
    for (qint32 iVertex = 0; iVertex < rootArray.size(); iVertex++)
    {
        if (!rootArray[iVertex].isObject())
            return false;
        // Get weights and indices fields
        QJsonArray jsonWeights = rootArray[iVertex].toObject().value("weight").toArray();
        QJsonArray jsonIndices = rootArray[iVertex].toObject().value("index").toArray();
        if ((jsonWeights.size() != VERTEX_BONES_MAX) || (jsonIndices.size() != VERTEX_BONES_MAX))
            return false;

        // Convert JSON data to skeleton and store it
        for (qint32 iRow = 0; iRow < VERTEX_BONES_MAX; iRow++)
        {
            // Bone index, beginning from 1
            quint32 boneIndex = jsonIndices[iRow].toInt();
            if (boneIndex == 0)
                break;
            m_mesh.vertices[iVertex].weights[iRow] = {boneIndex, jsonWeights[iRow].toDouble()};
        }
    }

    return true;
}
