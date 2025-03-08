#ifndef CSKELETON_HPP
#define CSKELETON_HPP

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

#include "meshprocessor.hpp"

// Class to store bones and apply transform to mesh
class CSkeleton
{
public:
    CSkeleton(CMeshProcessor &meshProcessor);
    // Load skeleton
    bool loadSkeleton(const QString &filename);
    // Load transforms
    bool loadTransforms(const QString &inverseFilename, const QString &newFilename);

protected:
    // Constants
    // Transform matrix length
    static constexpr qint32 TRANSFORM_MATRIX_LENGTH {16};

    // Types
    // Bone as array of vertices and their bones
    typedef std::vector<std::pair<qint32, float>> TBone;
    // Transform matrix
    typedef struct {float values[TRANSFORM_MATRIX_LENGTH];} TTransformMatrix;

    // Fields
    // Reference to mesh processor
    CMeshProcessor &m_meshProcessor;
    // Array of bones
    std::vector<TBone> m_bones;
    // Inverse transform
    std::vector<TTransformMatrix> m_inverseTransform;
    // New transform
    std::vector<TTransformMatrix> m_newTransform;

    // Methods
    // Load transform from file
    bool loadTransform(const QString &filename, std::vector<TTransformMatrix> &transform);
};

#endif
