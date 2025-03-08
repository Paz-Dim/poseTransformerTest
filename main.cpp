#include <QCoreApplication>
#include <QDebug>

#include "meshprocessor.hpp"
#include "skeleton.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Pose transformer start";

    // Mesh
    CMeshProcessor meshProcessor;
    // Load origin mesh
    if (!meshProcessor.load("Data/test_mesh.obj"))
    {
        qDebug() << "Can't load mesh";
        return a.exec();
    }

    // Skeleton
    CSkeleton skeleton(meshProcessor);
    // Load sekelton
    if (!skeleton.loadSkeleton("Data/bone_weight.json"))
    {
        qDebug () << "Can't load skeleton";
        return a.exec();
    }
    // Load matrices
    if (!skeleton.loadTransforms("Data/inverse_bind_pose.json", "Data/new_pose.json"))
    {
        qDebug () << "Can't load transforms";
        return a.exec();
    }

    // Apply loaded transforms
    skeleton.applyTransforms();

    // Save modified mesh
    if (!meshProcessor.save("/tmp/modifiedMesh.obj"))
    {
        qDebug() << "Can't save mesh";
        return a.exec();
    }

    return a.exec();
}
