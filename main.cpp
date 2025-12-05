#include "mainwindow.h"
#include <QApplication>
#include <allegro5/allegro5.h>

int main(int argc, char **argv)
{
    al_init()

    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
