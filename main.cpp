#include <QCoreApplication>
#include <QElapsedTimer>
#include <QDebug>

#include "skeletalMesh.hpp"
#include "skeletalTransform.hpp"

static const QString MESH_FILENAME {"Data/test_mesh.obj"};
static const QString WEIGHTS_FILENAME {"Data/bone_weight.json"};
static const QString INVERSE_POSE_FILENAME {"Data/inverse_bind_pose.json"};
static const QString NEW_POSE_FILENAME {"Data/new_pose.json"};
static const QString RESULT_FILENAME {"/tmp/modifiedMesh.obj"};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Pose transformer start";

    // Mesh
    CSkeletalMesh meshProcessor;
    // Load origin mesh
    if (!meshProcessor.load(MESH_FILENAME, WEIGHTS_FILENAME))
    {
        qDebug() << "Can't load mesh";
        return a.exec();
    }

    // Transform
    CSkeletalTransform transform(meshProcessor);
    // Load matrices
    if (!transform.loadTransforms(INVERSE_POSE_FILENAME, NEW_POSE_FILENAME))
    {
        qDebug () << "Can't load transforms";
        return a.exec();
    }

    QElapsedTimer nsecsTimer;
    nsecsTimer.start();

    // Apply loaded transforms
    transform.applyTransforms();

    qDebug() << nsecsTimer.nsecsElapsed() << "nanoseconds";

    // Save modified mesh
    if (!meshProcessor.saveMesh(RESULT_FILENAME))
    {
        qDebug() << "Can't save mesh";
        return a.exec();
    }

    return a.exec();
}
