#include "changelog_list.h"

changelog_list::changelog_list(QObject *parent) :
        QObject(parent),
        QList<changelog *> ()
{

}

QStringList changelog_list::titles()
{
        QStringList offset;
        for(int i = 0; i < size(); i++)
                offset << at(i)->title();
        return offset;
}
