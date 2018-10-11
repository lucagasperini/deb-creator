#ifndef DEBCREATOR_H
#define DEBCREATOR_H

#include "multiprocess.h"
#include "define.h"
#include "package.h"
#include "changelog.h"
#include "git.h"

#include <QtSql/QSqlDatabase>

class debcreator : public QObject
{
        Q_OBJECT
public:
        /**
         * @brief debcreator default constructor
         * @param file path of SQLITE3 database file
         * @param parent default qt parent
         */
        explicit debcreator(const QString &file_db = "", QObject *parent = nullptr);
        /**
         * @brief package create the package using QProcess to dpkg
         * @param control is the control text
         * @param outputfile the path where package is created
         */
        QByteArray pkg_create(const QByteArray &control, const QString &outputfile);
        /**
         * @brief db_insert insert into the database all variables in this class
         * @return if insert routine it's ok will return true otherwise false
         */
        bool db_insert();
        /**
         * @brief db_fetch fetch package name on the database
         * @return list of packages name of database
         */
        QStringList db_fetch();
        /**
         * @brief db_fetch check if the package is on the database, if yes it will put all data into the variables in this class
         * @param pkg is the name of the package to find
         * @return if package don't exist or something is wrong, it will return false, otherwise true
         */
        bool db_fetch(const QString &pkg);
        /**
         * @brief db_exists check if the package is on the database
         * @param pkg is the name of the package to find
         * @return if package don't exist it will return false, otherwise true
         */
        bool db_exists(const QString &pkg);
        /**
         * @brief db_remove remove the package on the database
         * @param pkg is the name of the package to find
         * @return if package don't exist it will return false, otherwise true
         */
        bool db_remove(const QString &pkg);
        static bool file_write(const QString &filename, const QString &data);
        static QByteArray file_read(const QString &filename);

        multiprocess* m_process;

        QSqlDatabase* m_db;
        package *m_pkg;
        changelog *m_changelog;
        QString m_build; //REVIEW?
};

#endif // DEBCREATOR_H
