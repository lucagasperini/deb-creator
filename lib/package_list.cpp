#include "package_list.h"

package_list::package_list(QObject *parent) :
        QObject(parent),
        QList<package*>()
{

}

QStringList package_list::names()
{
        QStringList offset;
        for(int i = 0; i < size(); i++) {
                offset << at(i)->m_name;
        }
        return offset;
}

package* package_list::byname(const QString &name)
{
        package *buffer ;
        for(int i = 0; i < size(); i++) {
                buffer = at(i);
                if(buffer->m_name == name)
                        return buffer;
        }
        return nullptr;
}
