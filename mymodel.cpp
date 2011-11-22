#include "mymodel.h"

MyModel::MyModel(QObject *parent) :
    QAbstractTableModel(parent)
{
};

int MyModel::rowCount(const QModelIndex &parent) const
{
}

int MyModel::columnCount(const QModelIndex &parent) const
{
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
}
