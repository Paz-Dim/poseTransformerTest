#ifndef CMESHPROCESSOR_HPP
#define CMESHPROCESSOR_HPP

#include <QCoreApplication>
#include <fstream>

#include "obj_io.h"

// Mesh processor, load/save mesh
class CMeshProcessor
{
public:
    // Types
    // 3-float vector
    struct FVector
    {
      float X {0.0f};
      float Y {0.0f};
      float Z {0.0f};
    };
    // Mesh (vertices positions and faces)
    struct FMesh
    {
      std::vector<FVector> vertices;
      std::vector<quint32> indices;
    };

    // Methods
    // Load mesh from file
    bool load(const QString &filename);
    // Save mesh into file
    bool save(const QString &filename);
    // Get reference to vertices array
    std::vector<FVector> &getVertices();

protected:
    // Types
    // Position type
    typedef thinks::ObjPosition<float, 3> TPos;
    // Face type
    typedef thinks::ObjIndex<uint32_t> TObjIndex;
    typedef thinks::ObjTriangleFace<TObjIndex> TFace;

    // Fields
    // Processed mesh
    FMesh m_mesh;
    // Data counters
    quint32 m_posCounter {0};

    // Read callbacks
    void readPosCallback(const thinks::ObjPosition<float, 3> &pos);
    void readFaceCallback(const TFace &face);
};

#endif
