#include "mainwindow.h"

int main(int argc,char **argv)
{
    QApplication app(argc,argv);

    mainwindow *win = new mainwindow;
    win->show();

    return app.exec();
}
