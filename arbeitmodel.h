#ifndef ARBEITMODEL_H
#define ARBEITMODEL_H

#include <QAbstractTableModel>

class ArbeitModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ArbeitModel( QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

};

#endif // ARBEITMODEL_H
