#ifndef PACKAGE_LIST_H
#define PACKAGE_LIST_H

#include "package.h"

#include <QList>

class package_list : public QObject, public QList<package*>
{
        Q_OBJECT
public:
        package_list(QObject *parent = nullptr);
        QStringList names();
        package* byname(const QString &name);
};

#endif // PACKAGE_LIST_H
