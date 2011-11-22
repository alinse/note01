#include "mainwidget.h"
// #include "arbeitmodel.h"
#include "mymodel.h"

#include <QDir>
#include <QtGui>
#include <QDebug>
#include <QErrorMessage>
#include <QString>
#include <QStringList>
#include <stdio.h>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>

#include <QDomDocument>
#include <QXmlQuery>
#include <QXmlFormatter>
#include <QtXmlPatterns>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    // Feste  Pfade
    APP_PATH     = QString(QDir::homePath() + "/Noten/");
    KLASSEN_PATH = APP_PATH         + "_klassen/";
    //
    // Alle Klassen aus "Noten/_klassen" einlesen
    //
    QDir klassenDir( KLASSEN_PATH );

    QString homePath = QDir::homePath();
    if ( ! klassenDir.cd( KLASSEN_PATH )) {
        qDebug() << "Kann nicht zu '" + KLASSEN_PATH + "' wechseln.";
    }
    QStringList klassen_namen = klassenDir.entryList(QStringList("*.klasse"));


    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->setVerticalSpacing(5);
    mainLayout->setHorizontalSpacing(5);

    QGroupBox *groupKlassenArbeitenAuswahl = new QGroupBox("Arbeitenauswahl");


    popupButtonKlassen = new QComboBox();
    popupButtonArbeiten = new QComboBox();

    textEdit = new QTextEdit();
    mainLayout->addWidget( textEdit, 1, 0 );

    connect(popupButtonKlassen, SIGNAL(currentIndexChanged(QString)), this, SLOT(klasseGewaehlt(QString)));
    for ( int i=0; i < klassen_namen.size(); i++) {
        QString klasse = (klassen_namen.at(i).split('.'))[0];
        popupButtonKlassen->addItem(klasse);
    }

    // popupButtonArbeiten->addItem("<-- Klasse auswählen ...");

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(popupButtonKlassen);
    hbox->addWidget(popupButtonArbeiten);
    groupKlassenArbeitenAuswahl->setLayout( hbox );

    mainLayout->addWidget( groupKlassenArbeitenAuswahl, 0, 0, 1, 3, 0  );
    // mainLayout->setRowStretch(0,0);


    //
    // Tabelle
    //
    tableView = new QTableView( this );
    mainLayout->addWidget( tableView, 1,1);

    setLayout( mainLayout );
}

///////////////////////////////////////////////////////////////////////////////////
void MainWidget::klasseGewaehlt(QString sKlasse)
{
    qDebug() << "MainWidgt::klasseGewaehlt(" + sKlasse + ")";
    // Für den Neuaufbau der Arbeiten-Liste keine Signale ...
    disconnect(popupButtonArbeiten, 0, this, 0); // @todo: Warum ist das notwendig?
                                                 //  Signal für Arbeit feuert ohne Wert!

    QString fileName = KLASSEN_PATH + sKlasse+".klasse";
    qDebug() << "  Klassen-Datei: " << fileName;
    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), "klasseGewaehlt(): Could not open file: "+fileName);
        return;
    }
    QTextStream in(&file);
    textEdit->setText(in.readAll());
    file.close();

    // Arbeiten einlesen
    popupButtonArbeiten->clear();
    QString sArbeitenDir = APP_PATH + sKlasse;
    QDir dirArbeiten(sArbeitenDir);

    QStringList arbeiten_namen = dirArbeiten.entryList(QStringList("*.xml"));
    for ( int i=0; i < arbeiten_namen.size(); i++) {
        QString arbeitenParts = (arbeiten_namen.at(i).split('.'))[0];
        qDebug() << arbeiten_namen.at(i) << " (->" + arbeitenParts + ")";
        popupButtonArbeiten->addItem(arbeitenParts);
    }

    connect(popupButtonArbeiten, SIGNAL(currentIndexChanged(QString)), this, SLOT(arbeitGewaehlt(QString)));

}


class MyItem : public QStandardItem
{
public:
    MyItem( QString s) : QStandardItem(s) {};

    QVariant data( int role = Qt::UserRole + 1 ) const
    {
        // qDebug() << "MyItem::data()";
        return QStandardItem::data( role );
    }
    void setData ( const QVariant & value, int role = Qt::UserRole + 1 )
    {
        qDebug() << "MyItem::setData(" << value << ")";
        QStandardItem::setData( value, role );
    }
};

///////////////////////////////////////////////////////////////////////////////////
bool MainWidget::readXMLFile( QIODevice *device )
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    domDocument = new QDomDocument();
    if (!domDocument->setContent(device, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return false;
    }

    QDomElement root = domDocument->documentElement();
    if (root.tagName() != "arbeit") {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("The file is not an XBEL file."));
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "1.0") {
        QMessageBox::information(window(), tr("Noten"),
                                 tr("The file is not an XBEL version 1.0 "
                                    "file."));
        return false;
    }

//    // clear();

//    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    QDomElement child = root.firstChildElement("klasse");
    qDebug() << "Erstes Kind: Type = " << child.nodeType();
    while (!child.isNull()) {
    //    parseFolderElement(child);
        child = child.nextSiblingElement("folder");
    }

//    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
//            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    return true;

}

///////////////////////////////////////////////////////////////////////////////////
void MainWidget::arbeitGewaehlt( QString sArbeit)
{
    QString sAktuelleKlasse(popupButtonKlassen->currentText());
    QString sAktuelleArbeit(popupButtonArbeiten->currentText());

    QString fileName = APP_PATH + sAktuelleKlasse + "/" + sArbeit+".xml";
    qDebug() << "Arbeit-Datei: " << fileName;
    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), "arbeitGewaehlt(): Could not open file: "+fileName);
        return;
    }
    textEdit->setText(file.readAll());

    QXmlQuery queryAufgaben;
    queryAufgaben.setQuery("doc('"+fileName+"')/arbeit/aufgaben/aufgabe/string()");

    if (!queryAufgaben.isValid()) {
        qDebug() << "invalid query (Aufgaben)!";
        return;
    }
    QStringList *sListAufgaben = new QStringList();
    if (!queryAufgaben.evaluateTo(sListAufgaben)) {
        qDebug() << "invalid evaluateTo (Aufgaben)!";
        return;
    }

    QXmlQuery querySchueler;
    querySchueler.setQuery("doc('"+fileName+"')/arbeit/schueler/@name/string()");

    if (!querySchueler.isValid()) {
        qDebug() << "invalid query (Schueler)!";
        return;
    }
    QStringList *sListSchueler = new QStringList();
    if (!querySchueler.evaluateTo(sListSchueler)) {
        qDebug() << "invalid evaluateTo (Schueler)!";
        return;
    }

    //
    // Tabelle konstruieren
    //
     int nrow = sListSchueler->size();
     int ncol = sListAufgaben->size();

    //create model
    //QStandardItemModel *model = new QStandardItemModel( nrow, ncol, this );

     MyModel *model = new MyModel( this );
    //
    // Tabelle mit Werten füllen
    //

    //create QTableview Header
    for (int i=0; i<ncol; i++)
        model->setHorizontalHeaderItem( i, new QStandardItem( sListAufgaben->at(i)) );

    for (int i=0; i<nrow; i++)
        model->setVerticalHeaderItem( i, new QStandardItem( sListSchueler->at(i)) );

    //fill model value
    for( int r=0; r<nrow; r++ )
    {
        for( int c=0; c<ncol; c++)
        {
            QString sstr = "[ " + QString::number(r) + " , " + QString::number(c) + " ]";
            // QStandardItem *item = new QStandardItem(QString("Idx ") + sstr);
            MyItem *myItem = new MyItem(QString("Idx ") + sstr);
            model->setItem(r, c, myItem );
        }
    }

    //set model

    tableView->setModel(model);
    tableView->resizeColumnsToContents();
}
