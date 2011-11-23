#include "mymodel.h"
#include <QDebug>
#include <QFile>
#include <QtGui>
#include <QMessageBox>
#include <QXmlQuery>
#include <QXmlFormatter>
#include <QtXmlPatterns>


///////////////////////////////////////////////////////////////////////////////////
MyModel::MyModel(QObject *parent, QString inputXmlFilename) :
    QAbstractTableModel(parent)
{
    QString fileName = inputXmlFilename;

    /*
    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, tr("Error"), "arbeitGewaehlt(): Could not open file: "+fileName);
        return;
    }
    QTextStream in(&file);
    QString sXml = in.readAll();
    */

    QXmlQuery queryAufgaben;
    // queryAufgaben.setFocus(sXml);
    queryAufgaben.setQuery("doc('"+fileName+"')/arbeit/aufgaben/aufgabe/string()");

    if (!queryAufgaben.isValid()) {
        qDebug() << "invalid query (Aufgaben)!";
        return;
    }
    sListAufgaben = new QStringList();
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
    sListSchueler = new QStringList();
    if (!querySchueler.evaluateTo(sListSchueler)) {
        qDebug() << "invalid evaluateTo (Schueler)!";
        return;
    }

};

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

        return QString("[ " + QString::number(index.row()) + " , " + QString::number(index.column()) + " ]");
    case Qt::CheckStateRole:
        return QVariant();
    }
    return QVariant();
}


