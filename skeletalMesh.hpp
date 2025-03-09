#ifndef CMESHPROCESSOR_HPP
#define CMESHPROCESSOR_HPP

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <fstream>

#include "obj_io.h"

// Skeletal mesh processor, load/save mesh, contains bone weights
class CSkeletalMesh
{
public:
    // Constants
    // Maximum number of vertex bones
    static constexpr quint8 VERTEX_BONES_MAX {4};

    // Types
    // 3-float vector
    struct FVector
    {
      float X {0.0f};
      float Y {0.0f};
      float Z {0.0f};
    };
    // Vertex: location, bone weights
    struct FVertex
    {
        FVector position {0.0f};
        // Array of weights <iBone, weight>, 4 maximum, iBone starts from 1, 0 - not used
        std::pair<qint32, float> weights[VERTEX_BONES_MAX];
    };
    // Mesh (vertices positions and faces)
    struct FMesh
    {
      std::vector<FVertex> vertices;
      std::vector<quint32> indices;
    };

    // Methods
    // Load skeletal mesh from files
    bool load(const QString &meshFilename, const QString &skeletonFilename);
    // Save mesh into file
    bool saveMesh(const QString &filename);
    // Get reference to vertices array
    std::vector<FVertex> &getVertices();

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

    // Methods
    // Load mesh
    bool loadMesh(const QString &filename);
    // Read callbacks
    void readPosCallback(const thinks::ObjPosition<float, 3> &pos);
    void readFaceCallback(const TFace &face);
    // Load skeleton
    bool loadSkeleton(const QString &filename);
};

#endif
