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

/*class NodeWrapper
{
public:
    NodeWrapper( QDomeNode &node );

private:
    QDomNode &node;

};
*/

/*
QDebug operator<<(QDebug dbg, const  QDomNode &n)
{
    dbg.nospace() << "nodeName=" << n.nodeName() << "(Type=" << n.nodeType() << ")"
                  << " (Value=" << n.nodeValue() << "): " << n.toElement().text();

    dbg.nospace() << " [";
    QDomNamedNodeMap namedNodeMapAttributes = n.attributes();
    for ( int i=0; i<namedNodeMapAttributes.length(); i++ ) {
        QDomNode item = namedNodeMapAttributes.item(i);
        dbg.nospace() << item.nodeName() << "/"<< item.nodeValue(); // << "/" << item.
    }
    dbg.nospace() << "]";
    return dbg.space();
}

void MyModel::processAufgabe( QDomNode n, QString sPrefix )
{
    qDebug() << n;

}

void MyModel::processAufgaben( QDomNode n, QString sPrefix )
{

    qDebug() << "Aufgaben: Name=" << n.nodeName() << "  (Type: " << n.nodeType() << ")";
    if ( n.hasChildNodes()) {
        QDomNodeList list = n.childNodes();
        for ( uint i=0; i < list.length(); i++ ) {
            QDomNode a = list.item(i);
            qDebug() << a;
            sListAufgaben->append( a.toElement().text() );
         }
    }
    else {
        QMessageBox::critical(0, tr("Error"), "<aufgaben> ohne Kinder!");
    }
}

void MyModel::processSchueler( QDomNode n, QString sPrefix )
{

    NodeVector schuelerzeile;
    qDebug() << "Schüler: Name=" << n.nodeName() << "  (Type: " << n.nodeType() << ")";
    QDomNamedNodeMap namedNodeMapAttributes = n.attributes();
    if ( namedNodeMapAttributes.contains("name")) {
        sListSchueler->append( QString( namedNodeMapAttributes.namedItem("name").nodeValue()));
        vectorNodes.append( schuelerzeile );
    }
    else {
        QMessageBox::critical(0, tr("Error"), "Schüler ohne name-Attribut!");
    }

    if ( n.hasChildNodes()) {
        QDomNodeList list = n.childNodes();
        for ( uint i=0; i < list.length(); i++ ) {
            QDomNode a = list.item(i);
            qDebug() << a;
            QDomNamedNodeMap namedNodeMapAttributes = n.attributes();

            QDomElement e = n.toElement();

            qDebug() << e.attribute("bezeichner");
            qDebug() << e.attribute("punkte");

            QString aufgabe = namedNodeMapAttributes.namedItem("bezeichner").nodeValue();
            QString punkte = namedNodeMapAttributes.namedItem("punkte").nodeValue();
            int iAufgabePos = sListAufgaben->indexOf(aufgabe);
            schuelerzeile[iAufgabePos] = a;
            // processSchuelerAufgaben( , sPrefix + " CoS ", schuelerzeile );

         }
    }
    else {
        QMessageBox::critical(0, tr("Error"), "<Schueler> ohne Kinder!");
    }

}

void MyModel::processSchuelerAufgaben( QDomNode n, QString sPrefix, DomNodeVector *schuelerzeile )
{
    qDebug() << sPrefix << n;
}

void MyModel::processNode( QDomNode n, QString sPrefix )
{
    qDebug() << "Name=" << n.nodeName() << "  (Type: " << n.nodeType() << ")";
    if (n.isElement()) {
        qDebug() << sPrefix << n.toElement().tagName() << endl;
        if ( n.nodeName() == "aufgaben") {
            processAufgaben( n, sPrefix + ">> " );
            return;
        }
        else if ( n.nodeName() == "schueler") {
            processSchueler( n, sPrefix + "## " );
            return;
        }
    }
    if ( n.hasChildNodes()) {
        QDomNodeList list = n.childNodes();
        for ( uint i=1; i <= list.length(); i++ ) {
            processNode( list.item(i), sPrefix + "  " );
         }
    }
}
*/

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

/*    QDomDocument d;
    d.setContent( sXml );

    QDomNode n = d.firstChild();
    while ( ! n.isNull()) {
        processNode( n );
        n = n.nextSibling();
    }
*/

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


