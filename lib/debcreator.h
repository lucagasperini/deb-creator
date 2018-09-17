#ifndef DEBCREATOR_H
#define DEBCREATOR_H

#include "define.h"

#include <QObject>
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
        explicit debcreator(const QString &file_db, QObject *parent = nullptr);
        /**
         * @brief control generate a control file from variables in this class
         * @return return the control text
         */
        QString control();
        /**
         * @brief changelog generate and store changelog in this class
         * @param text user log in changelog
         * @return if changelog is created correctly return true otherwise false
         */
        bool changelog(const QString &text, const QString &status, const QString &urgency);
        /**
         * @brief package create the package using QProcess to dpkg
         * @param control is the control text
         * @return return the text of input stream from dpkg process
         */
        QString package(const QString &control);
        /**
         * @brief fetch_changelog fetch previous changelog
         * @return list of changelogs
         */
        QStringList fetch_changelog(const QString &file);
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
         * @brief git_fetch_user fetch user.email and user.name from git config
         * @return the format from git is user.name <user.email>, empty string if git is not installed or git is not configurated
         */
        static QString git_fetch_user();
        /**
         * @brief date_fetch fetch date in standard format from locale
         * @return the formated string of date
         */
        static QString date_fetch();

        QSqlDatabase* m_db;

        QString m_package;
        QString m_version;
        QString m_arch;
        QString m_depends;
        QString m_maintainer;
        QString m_desc_title;
        QString m_desc_body;
        QString m_homepage;
        QString m_uploaders;
        QString m_section;
        QString m_source;
        QString m_replace;

        QString m_changelog;
        QString m_control;

        QString m_dir;
        QString m_outputfile;
};

#endif // DEBCREATOR_H
