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
    CMeshProcessor();
    // Load mesh from file
    void load(const QString &filename);

protected:
    // Types
    // Position type
    typedef thinks::ObjPosition<float, 3> TPos;
    // Face type
    typedef thinks::ObjTriangleFace<thinks::ObjIndex<std::uint16_t>> TFace;
    // Normal type
    typedef thinks::ObjNormal<float> TNormal;

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
