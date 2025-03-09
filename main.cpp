#include <QCoreApplication>
#include <QElapsedTimer>
#include <QDebug>

#include "skeletalMesh.hpp"
#include "skeletalTransform.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Pose transformer start";

    // Mesh
    CSkeletalMesh meshProcessor;
    // Load origin mesh
    if (!meshProcessor.load("Data/test_mesh.obj", "Data/bone_weight.json"))
    {
        qDebug() << "Can't load mesh";
        return a.exec();
    }

    // Transform
    CSkeletalTransform transform(meshProcessor);
    // Load matrices
    if (!transform.loadTransforms("Data/inverse_bind_pose.json", "Data/new_pose.json"))
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
    if (!meshProcessor.saveMesh("/tmp/modifiedMesh.obj"))
    {
        qDebug() << "Can't save mesh";
        return a.exec();
    }

    return a.exec();
}
