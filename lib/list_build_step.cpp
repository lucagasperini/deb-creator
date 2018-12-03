#include "list_build_step.h"

list_build_step::list_build_step(QObject *parent) :
        QObject(parent),
        QList<build_step *> ()
{

}

QStringList list_build_step::titles()
{
        QStringList offset;
        for(int i = 0; i < size(); i++)
                offset << at(i)->m_app;
        return offset;
}
