#include "mymodel.h"
#include <QDebug>
#include <QFile>
#include <QtGui>
#include <QMessageBox>
#include <QXmlQuery>
#include <QXmlFormatter>
#include <QtXmlPatterns>
#include <QDomNode>
#include <QTextStream>


class Node
{
public:
    Node( int i, int j) : _row(i), _col(j) { qDebug() << "Neuer: " << text(); }
    QString text() {
        return QString("Node(row: %1,col: %2)").arg(_row).arg(_col);
    }
private:
    int _row, _col;
    int _iPunkte;
    int note();
};

///////////////////////////////////////////////////////////////////////////////////
MyModel::MyModel(QObject *parent, QString inputXmlFilename) :
    QAbstractTableModel(parent)
{
    readXmlFile( inputXmlFilename );
    QXmlQuery queryAufgaben;
    queryAufgaben.setFocus(sXml);
    queryAufgaben.setQuery("/arbeit/aufgaben/aufgabe/string()");

    if (!queryAufgaben.isValid()) {
        qDebug() << "invalid query (Aufgaben)!";
        return;
    }

    sListAufgaben = new QStringList();
    if (!queryAufgaben.evaluateTo(sListAufgaben)) {
        qDebug() << "invalid evaluateTo (Aufgaben)!";
        return;
    }

    ////////////////////////////
    QXmlQuery querySchueler;
    querySchueler.setFocus(sXml);
    querySchueler.setQuery("/arbeit/schueler/@name/string()");

    if (!querySchueler.isValid()) {
        qDebug() << "invalid query (Schueler)!";
        return;
    }
    sListSchueler = new QStringList();
    if (!querySchueler.evaluateTo(sListSchueler)) {
        qDebug() << "invalid evaluateTo (Schueler)!";
        return;
    }

    for ( int row=0; row<sListSchueler->length(); row++ ) {
        NodeVector *d = new NodeVector;
        vectorNodes.append( *d );
        for ( int col=0; col<sListAufgaben->length(); col++) {
            d->append( new Node(row, col) );
        }
    }
    // vectorNodes.dumpObjectTree();
};

void MyModel::readXmlFile( QString inputXmlFilename )
{
    QString fileName = inputXmlFilename;

    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, tr("Error"), "arbeitGewaehlt(): Could not open file: "+fileName);
        return;
    }
    QTextStream in(&file);
    sXml = in.readAll();
}

///////////////////////////////////////////////////////////////////////////////////
QVariant MyModel::headerData( int section, Qt::Orientation orientation,  int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)  {
            return sListAufgaben->at(section);
        }
        else if (orientation == Qt::Vertical) {
            return sListSchueler->at(section);
        }
        else{
            Q_ASSERT(0);
        }
    }
    return QVariant();
}

///////////////////////////////////////////////////////////////////////////////////
int MyModel::rowCount(const QModelIndex &parent) const
{
    return sListSchueler->size();

}

///////////////////////////////////////////////////////////////////////////////////
int MyModel::columnCount(const QModelIndex &parent) const
{
    return sListAufgaben->size();
}

///////////////////////////////////////////////////////////////////////////////////
QVariant MyModel::data(const QModelIndex &index, int role) const
{
    switch (role){

    case Qt::DisplayRole:
    {
        int row = index.row();
        int col = index.column();

        QString sSchueler = sListSchueler->at(row);
        QString sAufgabe = sListAufgaben->at(col);

        QXmlQuery queryNote;
        queryNote.setFocus(sXml);
        QString sQuery = "/arbeit/schueler[@name=\""+sSchueler+"\"]/aufgabe[@bezeichner=\""+sAufgabe+"\"]/@punkte/string()";
        queryNote.setQuery(sQuery);

        if (!queryNote.isValid()) {
            qDebug() << "invalid query (queryNote)!";
            qDebug() << sQuery;
            return "invalid query (queryNote)!";
        }
        QString s="<leer>";
        if (!queryNote.evaluateTo(&s)) {
            qDebug() << "invalid evaluateTo (queryNote)!";
            return "invalid evaluateTo (queryNote)!";
        }
        return s;
        break;
    }
    case Qt::CheckStateRole:
        return QVariant();
        break;
    }
    return QVariant();
}


