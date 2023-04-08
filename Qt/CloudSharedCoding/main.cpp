#include "cloudsharedcoding.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CloudSharedCoding w;
    w.show();

    return a.exec();
}
