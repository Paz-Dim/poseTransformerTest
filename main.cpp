#include <QCoreApplication>
#include <QDebug>

#include "meshprocessor.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Pose transformer start";

    CMeshProcessor meshProcessor;
    meshProcessor.load("Data/test_mesh.obj");

    // Modify
    auto &vertices = meshProcessor.getVertices();
    for (quint32 iVertex = vertices.size() / 3; iVertex < 2 * vertices.size() / 3; iVertex++)
    {
        vertices[iVertex].Y *= 2.0f;
        vertices[iVertex].Z += 0.05f;
    }

    meshProcessor.save("/tmp/modifiedMesh.obj");

    return a.exec();
}
