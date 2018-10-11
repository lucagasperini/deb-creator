#ifndef DEBCREATOR_H
#define DEBCREATOR_H

#include "multiprocess.h"
#include "define.h"
#include "package.h"
#include "changelog.h"
#include "git.h"
#include "filesystem.h"

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
        /**
         * @brief build_dir current build directory for stored package
         * @return string of build directory path
         */
        QString build_dir();
        /**
         * @brief file_write write data in selected filename
         * @param filename target file
         * @param data binary data to write
         * @return if cannot write in target file it will return false, otherwise true
         */
        static bool file_write(const QString &filename, const QString &data);
        /**
         * @brief file_read read binary data from target file
         * @param filename file where data is stored
         * @return data from target file
         */
        static QByteArray file_read(const QString &filename);

        multiprocess* m_process;

        QSqlDatabase* m_db;
        package *m_pkg;
        changelog *m_changelog;
};

#endif // DEBCREATOR_H
