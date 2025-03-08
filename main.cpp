#include <QCoreApplication>
#include <QDebug>

#include "meshprocessor.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Pose transformer start";

    CMeshProcessor meshProcessor;
    meshProcessor.load("Data/test_mesh.obj");

    return a.exec();
}
