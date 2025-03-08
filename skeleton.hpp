#ifndef CSKELETON_HPP
#define CSKELETON_HPP

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

#include "meshprocessor.hpp"
#include "mutil/mutil.h"

// Class to store bones and apply transform to mesh
class CSkeleton
{
public:
    CSkeleton(CMeshProcessor &meshProcessor);
    // Load skeleton
    bool loadSkeleton(const QString &filename);
    // Load transforms
    bool loadTransforms(const QString &inverseFilename, const QString &newFilename);
    // Apply transforms
    void applyTransforms();

protected:
    // Constants
    // Transform matrix length
    static constexpr qint32 TRANSFORM_MATRIX_LENGTH {16};

    // Types
    // Array of bones for vertex: iVertex -> <iBone, weight>
    typedef std::vector<std::pair<qint32, float>> TVertexBone;

    // Fields
    // Reference to mesh processor
    CMeshProcessor &m_meshProcessor;
    // Array of vertices and assigned bones
    std::vector<TVertexBone> m_verticesBones;
    // Inverse transform
    std::vector<mutil::Matrix4> m_inverseTransform;
    // New transform
    std::vector<mutil::Matrix4> m_newTransform;

    // Methods
    // Load transform from file
    bool loadTransform(const QString &filename, std::vector<mutil::Matrix4> &transform);
};

#endif
