#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QDomNode>

/// List aller Punkte pro Schüler
class Node;

typedef QVector<Node *> NodeVector;


class MyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    /// XML-Datei für eine Arbeit
    MyModel(QObject *parent, QString inputXmlFilename);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:
    void textLoaded( QString );

private:
    void readXmlFile( QString filename );
    void processNode(QDomNode n, QString sPrefix = "  ");
    void processAufgaben(QDomNode n, QString prefix);
    void processAufgabe(QDomNode n, QString sPrefix = "  ");
    void processSchueler(QDomNode n, QString sPrefix);
    void processSchuelerAufgaben(QDomNode n, QString sPrefix, NodeVector *schuelerzeile);

    QString sXml;
    QStringList *sListAufgaben;
    QStringList *sListSchueler;
    QVector<NodeVector> vectorNodes;
};
#endif // MYMODEL_H
