#ifndef CSKELETON_HPP
#define CSKELETON_HPP

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

#include "skeletalMesh.hpp"
#include "mutil/mutil.h"

/* Class to apply transform to skeletal mesh. Uses mutil libarry for vector/matrix math.
 * Modifies skeletal mesh vertices directly. */
class CSkeletalTransform
{
public:
    // Methods
    CSkeletalTransform(CSkeletalMesh &skeletalMesh);
    // Load transforms
    bool loadTransforms(const QString &inverseFilename, const QString &newFilename);
    /* Apply transforms
     * TODO: Modifies vertices in-place. For real usage the origin vertices
     * array should be read-only (as reference pose), function will create and return modified array. */
    void applyTransforms();

protected:
    // Constants
    // Transform matrix length
    static constexpr qint32 TRANSFORM_MATRIX_LENGTH {16};

    // Fields
    // Reference to skeletal mesh
    CSkeletalMesh &m_skeletalMesh;
    /* Array of pre-multiplied transforms for all bones.
     * TODO: For some reason, bones start from 1th element. */
    // Combined transforms for bones: iBone -> new * inverse
    std::vector<mutil::Matrix4> m_transforms;

    // Methods
    // Load transform from file
    bool loadTransform(const QString &filename, std::vector<mutil::Matrix4> &transform);
};

#endif
