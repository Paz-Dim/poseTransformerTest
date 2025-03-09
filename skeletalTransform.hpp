#ifndef CSKELETON_HPP
#define CSKELETON_HPP

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

#include "skeletalMesh.hpp"
#include "mutil/mutil.h"

// Class to apply transform to skeletal mesh
class CSkeletalTransform
{
public:
    CSkeletalTransform(CSkeletalMesh &skeletalMesh);
    // Load transforms
    bool loadTransforms(const QString &inverseFilename, const QString &newFilename);
    // Apply transforms
    void applyTransforms();

protected:
    // Constants
    // Transform matrix length
    static constexpr qint32 TRANSFORM_MATRIX_LENGTH {16};

    // Fields
    // Reference to skeletal mesh
    CSkeletalMesh &m_skeletalMesh;
    // Combined transforms: new * inversed
    std::vector<mutil::Matrix4> m_transforms;

    // Methods
    // Load transform from file
    bool loadTransform(const QString &filename, std::vector<mutil::Matrix4> &transform);
};

#endif
