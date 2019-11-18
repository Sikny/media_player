#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    if(argc > 1)
        w.loadMedia(argv[1]);
    w.show();
    return a.exec();
}
