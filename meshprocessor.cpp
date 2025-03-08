#include "meshprocessor.hpp"

CMeshProcessor::CMeshProcessor()
{
}


void CMeshProcessor::load(const QString &filename)
{
    // Remove old data
    m_mesh.vertices.clear();
    m_mesh.indices.clear();
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
    assert(ifs);
    const auto result = thinks::ReadObj(ifs, addPosition, addFace);
    ifs.close();

    // Some sanity checks
    assert(result.position_count == m_mesh.vertices.size() && "bad position count");
}


void CMeshProcessor::save(const QString &filename)
{
    // Positions
    m_posCounter = 0;
    auto posMapper = [this]()
    {
        // No vertices
        if (m_posCounter == m_mesh.vertices.size())
            return thinks::ObjEnd<TPos>();

        // Map indicates that additional positions may be available after this one.
        auto result = thinks::ObjMap(TPos(m_mesh.vertices[m_posCounter].X,
                                          m_mesh.vertices[m_posCounter].Y,
                                          m_mesh.vertices[m_posCounter].Z));
        m_posCounter++;
        return result;
    };

    // Faces
    quint32 indexCounter = 0;
    auto faceMapper = [this, &indexCounter]()
    {
        // Check that there are 3 more indices (trailing indices handled below).
        if ((m_mesh.indices.size() - indexCounter) < 3)
            return thinks::ObjEnd<TFace>();

        // Create a face from the mesh indices.
        auto result = thinks::ObjMap(TFace{TObjIndex(m_mesh.indices[indexCounter]),
                                           TObjIndex(m_mesh.indices[indexCounter + 1]),
                                           TObjIndex(m_mesh.indices[indexCounter + 2])});
        indexCounter += 3;
        return result;
    };

    // Open the OBJ file and pass in the mappers, which will be called
    // internally to write the contents of the mesh to the file.
    auto ofs = std::ofstream(filename.toStdString());
    assert(ofs);
    const auto result = thinks::WriteObj(ofs, posMapper, faceMapper);
    ofs.close();

    // Some sanity checks.
    assert(result.position_count == m_mesh.vertices.size() && "bad position count");
    assert(result.face_count == m_mesh.indices.size() / 3 && "bad face count");
    assert(indexCounter == m_mesh.indices.size() && "trailing indices");
}


std::vector<CMeshProcessor::FVector> &CMeshProcessor::getVertices()
{
    return m_mesh.vertices;
}


void CMeshProcessor::readPosCallback(const TPos &pos)
{
    // Check if we need a new vertex.
    if (m_mesh.vertices.size() <= m_posCounter)
        m_mesh.vertices.emplace_back();

    // Write the position property of current vertex
    m_mesh.vertices[m_posCounter] = {pos.values[0], pos.values[1], pos.values[2]};
    m_posCounter++;
}


void CMeshProcessor::readFaceCallback(const TFace &face)
{
    // Add triangle indices into the linear storage of our mesh class.
    m_mesh.indices.push_back(face.values[0].value);
    m_mesh.indices.push_back(face.values[1].value);
    m_mesh.indices.push_back(face.values[2].value);
}
