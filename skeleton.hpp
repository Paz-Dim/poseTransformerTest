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
    // Apply transforms
    void applyTransforms();

protected:
    // Constants
    // Transform matrix length
    static constexpr qint32 TRANSFORM_MATRIX_LENGTH {16};

    // Types
    // Array of bones for vertex: iVertex -> <iBone, weight>
    typedef std::vector<std::pair<qint32, float>> TVertexBone;
    // Transform matrix
    struct TTransformMatrix
    {
        float values[TRANSFORM_MATRIX_LENGTH] {0.0f};
    };
    // Transform coords
    struct TTransformCoords
    {
        float X {0.0f};
        float Y {0.0f};
        float Z {0.0f};
        float W {1.0f};

        TTransformCoords(){}
        TTransformCoords(const CMeshProcessor::FVector &vertex)
        {
            X = vertex.X;
            Y = vertex.Y;
            Z = vertex.Z;
        }
        CMeshProcessor::FVector getVector()
        {
            if (!qFuzzyIsNull(W))
            {
                X /= W;
                Y /= W;
                Z /= W;
            }
            return {X, Y, Z};
        }
        TTransformCoords operator *(float mul)
        {
            TTransformCoords result;
            result.X = X * mul;
            result.Y = Y * mul;
            result.Z = Z * mul;
            result.W = W * mul;
            return result;
        }
        TTransformCoords operator +(const TTransformCoords &term)
        {
            TTransformCoords result;
            result.X = X + term.X;
            result.Y = Y + term.Y;
            result.Z = Z + term.Z;
            result.W = W + term.W;
            return result;
        }
    };

    // Fields
    // Reference to mesh processor
    CMeshProcessor &m_meshProcessor;
    // Array of vertices and assigned bones
    std::vector<TVertexBone> m_verticesBones;
    // Inverse transform
    std::vector<TTransformMatrix> m_inverseTransform;
    // New transform
    std::vector<TTransformMatrix> m_newTransform;

    // Methods
    // Load transform from file
    bool loadTransform(const QString &filename, std::vector<TTransformMatrix> &transform);
    // Apply transform to coords
    TTransformCoords transformCoords(const TTransformMatrix &matrix, const TTransformCoords &coords) const;
};

#endif
