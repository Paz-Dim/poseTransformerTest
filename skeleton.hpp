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

protected:
    // Types
    // Bone as array of vertices and their bones
    typedef std::vector<std::pair<qint32, float>> TBone;

    // Fields
    // Reference to mesh processor
    CMeshProcessor &m_meshProcessor;
    // Array of bones
    std::vector<TBone> m_bones;
};

#endif
