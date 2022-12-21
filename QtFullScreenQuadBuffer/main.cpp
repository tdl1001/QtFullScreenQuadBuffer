#include "QtFullScreenQuadBuffer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtFullScreenQuadBuffer w;
    w.show();
    return a.exec();
}
