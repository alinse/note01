#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractTableModel>

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
    QString sXml;
    QStringList *sListAufgaben;
    QStringList *sListSchueler;
};
#endif // MYMODEL_H
