#include <QCoreApplication>
#include "Test.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DisplayThreadInfo("Test main");

    Test t;

    return a.exec();
}
