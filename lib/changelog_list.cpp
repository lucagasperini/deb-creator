#include "changelog_list.h"

changelog_list::changelog_list(QObject *parent) :
        QObject(parent),
        QList<changelog *> ()
{

}

QStringList changelog_list::titles() const
{
        QStringList offset;
        for(int i = 0; i < size(); i++)
                offset << at(i)->title();
        return offset;
}

QByteArray changelog_list::text() const
{
        QByteArray offset;
        for(int i = 0; i < size(); i++) {
                offset.append(at(i)->generate());
        }
        return offset;
}
