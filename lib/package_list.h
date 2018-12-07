#ifndef PACKAGE_LIST_H
#define PACKAGE_LIST_H

#include "package.h"

#include <QList>

class package_list : public QObject, public QList<package*>
{
        Q_OBJECT
public:
        package_list(QObject *parent = nullptr);
        /**
         * @brief names get all names from package_list
         * @return string list with names
         */
        QStringList names();
        /**
         * @brief byname find a package by name
         * @param name name of package to find
         * @return pointer to package found
         */
        package* byname(const QString &name);
};

#endif // PACKAGE_LIST_H
