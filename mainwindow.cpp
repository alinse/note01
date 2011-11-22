#include "mainwindow.h"
#include "mainwidget.h"

#include <QTableView>
// #include "arbeitmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

     MainWidget *mainWidget = new MainWidget( this );
     setCentralWidget(mainWidget);

     resize(1000, 700);
     setWindowTitle("Noten");
}
