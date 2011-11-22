
#include "arbeitmodel.h"
#include <QDebug>

ArbeitModel::ArbeitModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    qDebug() << "ArbeitModel::ArbeitModel() aufgerufen!";
}

int ArbeitModel::rowCount(const QModelIndex & /*parent*/) const
{
    qDebug() << "ArbeitModel::rowCount() aufgerufen!";
    return 2;
}

int ArbeitModel::columnCount(const QModelIndex & /*parent*/) const
{
    qDebug() << "ArbeitModel::columnCount() aufgerufen!";
    return 3;
}

QVariant ArbeitModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "\nArbeitModel::data aufgerufen:\n\t " << index;
    if (role == Qt::DisplayRole)
    {
       return QString("Row%1, Column%2")
                   .arg(index.row() + 1)
                   .arg(index.column() +1);
    }
    return QVariant();
}
