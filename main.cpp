#include "mainwindow.h"

#include <QApplication>
//#include <QMetaType>
int main(int argc, char *argv[]){
    QApplication a(argc, argv);
//    qRegisterMetaType<Edge>("Edge");
    MainWindow w;
    w.show();
    return a.exec();
}
