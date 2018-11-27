#ifndef PACKAGE_LIST_H
#define PACKAGE_LIST_H

#include "package.h"

#include <QList>

class package_list : public QList<package*>
{
public:
        package_list();
        QStringList names();
        package* byname(const QString &name);
};

#endif // PACKAGE_LIST_H
