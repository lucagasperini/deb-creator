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

build_step* list_build_step::find(const build_step* p)
{
        build_step* t;
        for(int i = 0; i < size(); i++) {
                t = at(i);
                if(p->m_app == t->m_app && p->m_arg == t->m_arg && p->m_dir == t->m_dir && p->m_pkg == t->m_pkg)
                        return t;
        }
        return nullptr;
}
