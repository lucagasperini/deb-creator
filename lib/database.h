#ifndef DATABASE_H
#define DATABASE_H

#include "package.h"

#include <QObject>
#include <QtSql/QSqlDatabase>

class database : public QObject
{
        Q_OBJECT
public:
        explicit database(QObject *parent = nullptr);
        explicit database(const QString &file_db, QObject *parent = nullptr);

        /**
         * @brief pkg_insert insert into the database all variables in this class
         * @return if insert routine it's ok will return true otherwise false
         */
        bool pkg_insert(const package *pkg);
        /**
         * @brief pkg_fetch fetch package name on the database
         * @return list of packages name of database
         */
        QStringList pkg_fetch();
        /**
         * @brief pkg_fetch check if the package is on the database, if yes it will put all data into the variables in this class
         * @param pkg is the name of the package to find
         * @return if package don't exist or something is wrong, it will return false, otherwise true
         */
        package *pkg_fetch(const QString &pkg);
        /**
         * @brief pkg_exists check if the package is on the database
         * @param pkg is the name of the package to find
         * @return if package don't exist it will return false, otherwise true
         */
        bool pkg_exists(const QString &pkg);
        /**
         * @brief pkg_remove remove the package on the database
         * @param pkg is the name of the package to find
         * @return if package don't exist it will return false, otherwise true
         */
        bool pkg_remove(const QString &pkg);

        QSqlDatabase* m_db;
};

#endif // DATABASE_H
