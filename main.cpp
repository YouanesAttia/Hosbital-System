#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    MainWindow w;
    w.setWindowTitle("AUC Hospital Management System");
    w.resize(1000, 700);
    w.show();
    
    return app.exec();
}
