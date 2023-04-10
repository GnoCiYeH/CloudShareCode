#include "welcomewidget.h"

welcomewidget::welcomewidget(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant welcomewidget::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex welcomewidget::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex welcomewidget::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int welcomewidget::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int welcomewidget::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant welcomewidget::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
