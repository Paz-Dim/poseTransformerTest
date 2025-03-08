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

    // Positions.
    auto add_position = thinks::MakeObjAddFunc<TPos>([this](const TPos &pos)
    {
        readPosCallback(pos);
    });
    // Faces
    auto add_face = thinks::MakeObjAddFunc<TFace>([this](const TFace &face)
    {
        readFaceCallback(face);
    });

    // Open the OBJ file and populate the mesh while parsing it.
    auto ifs = std::ifstream(filename.toStdString());
    assert(ifs);
    const auto result = thinks::ReadObj(ifs, add_position, add_face, nullptr, nullptr);
    ifs.close();

    // Some sanity checks.
    assert(result.position_count == m_mesh.vertices.size() && "bad position count");
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
