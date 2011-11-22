
#include <QtGui>
#include "mainwindow.h"
#include "arbeitmodel.h"

int main(int argv, char **args)
{
     QApplication app(argv, args);

     MainWindow window;
     window.show();

     return app.exec();

}
