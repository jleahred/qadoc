#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QApplication::setWindowIcon(QIcon(":/book.svg"));
    w.showMaximized();
    return a.exec();
}
