#ifndef DEBCREATOR_H
#define DEBCREATOR_H

#include "define.h"

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QProcess>
#include <QDir>

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
         * @brief control generate a control file from variables in this class
         * @return return the control text
         */
        QByteArray control();
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
        QString package(const QByteArray &control, const QString &outputfile = "");
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
         * @brief compile source code in @public m_build_dir and using @public m_build processes
         * @return Processes output stream
         */
        QByteArray compile();
        /**
         * @brief git_clone fetch git repo in directory @public m_dir + '/build'
         * @param url repo git
         * @return directory of files stored
         */
        QString git_clone(const QString &url);
        /**
         * @brief build_append add a step build in @public m_build
         * @param program path of the program
         * @param args arguments of the program
         * @param working_dir directory where source code is
         */
        void build_append(const QString &program, const QStringList &args, const QString &working_dir = "");
        /**
         * @brief build_clear erase all step build in @public m_build
         */
        void build_clear();
        /**
         * @brief build_is_empty check if @public m_build is empty
         * @return if there are step build it will return false, otherwise true
         */
        bool build_is_empty();
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
        /**
         * @brief calc_size calculate the size of package directory
         * @param _dir root directory of the package
         * @return integer 64 bit of size in byte
         */
        static qint64 calc_size(const QString &_dir);

        QSqlDatabase* m_db;
        QList<QProcess*> *m_build;

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

        QDir m_dir;
        QString m_build_dir;
};

#endif // DEBCREATOR_H
