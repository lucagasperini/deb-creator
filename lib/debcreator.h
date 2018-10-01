#ifndef DEBCREATOR_H
#define DEBCREATOR_H

#include "multiprocess.h"
#include "define.h"
#include "package.h"

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
         * @brief changelog generate and store changelog in this class
         * @param text user log in changelog
         * @return if changelog is created correctly return true otherwise false
         */
        bool changelog(const QString &text, const QString &status, const QString &urgency);
        /**
         * @brief package create the package using QProcess to dpkg
         * @param control is the control text
         */
        QByteArray pkg_create(const QByteArray &control, const QString &outputfile = "");
        /**
         * @brief fetch_changelog fetch previous changelog
         * @return list of changelogs
         */
        QStringList fetch_changelog(const QString &file);
        /**
         * @brief gen_outputfile generate a standard filename for package .deb
         * @return filename of the package
         */
        QString gen_outputfile();
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
         * @brief git_clone fetch git repo in directory @public m_dir + '/build'
         * @param url repo git
         * @return directory of files stored
         */
        static QString git_clone(const QString &url, QString directory = "");
        /**
         * @brief git_fetch_user fetch user.email and user.name from git config
         * @return the format from git is user.name <user.email>, empty string if git is not installed or git is not configurated
         */
        static QString git_fetch_user();
        /**
         * @brief date_fetch fetch date in standard format from locale
         * @return the formated string of date
         */
        static QString date_fetch();

        multiprocess* m_process;

        QSqlDatabase* m_db;
        package *m_pkg;
        QString m_changelog;
};

#endif // DEBCREATOR_H
