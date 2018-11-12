#ifndef DATABASE_H
#define DATABASE_H

#include "package.h"
#include "build_step.h"

#include <QObject>
#include <QtSql/QSqlDatabase>

class database : public QObject
{
        Q_OBJECT
public:
        explicit database(QObject *parent = nullptr);
        explicit database(const QString &file_db, QObject *parent = nullptr);

        /**
         * @brief pkg_insert insert into the database all variables
         * @param pkg is the package to insert
         * @return if insert routine it's ok will return true otherwise false
         */
        bool pkg_insert(const package *pkg);
        /**
         * @brief pkg_fetch fetch package name on the database
         * @return list of packages name of database
         */
        QMap<int,QString>* pkg_fetch();
        /**
         * @brief pkg_fetch check if the package is on the database, if yes it will put all data into the variables
         * @param pkg is the name of the package to find
         * @return if package don't exist or something is wrong, it will return false, otherwise true
         */
        package *pkg_fetch(int id);
        /**
         * @brief pkg_exists check if the package is on the database
         * @param pkg is the name of the package to find
         * @return if package don't exist it will return false, otherwise true
         */
        bool pkg_exists(int id);
        /**
         * @brief pkg_remove remove the package on the database
         * @param pkg is the name of the package to find
         * @return if package don't exist it will return false, otherwise true
         */
        bool pkg_remove(int id);
        /**
         * @brief build_insert insert into the database all variables
         * @param pkg is the name of the package to insert
         * @param step is the build_step to insert
         * @return if insert routine it's ok will return true otherwise false
         */
        bool build_insert(const QString &pkg, const build_step &step);
        /**
         * @brief build_fetch check if there are build_step on the database, if yes it will put all data on values list
         * @param pkg is the name of the package
         * @return if package don't exist or something is wrong, it will return a nullptr
         */
        QList<build_step *> *build_fetch(const QString &pkg);
        /**
         * @brief build_exists check if the build_step is on the database
         * @param pkg is the name of the package to find
         * @param step is the build_step to find
         * @return if package don't exist it will return false, otherwise true
         */
        bool build_exists(const QString &pkg, const build_step &step);
        /**
         * @brief build_remove remove the build_step on the database
         * @param pkg is the name of the package to find
         * @param step is the build_step to find
         * @return if step_build don't exist it will return false, otherwise true
         */
        bool build_remove(const QString &pkg, const build_step &step);
        /**
         * @brief build_insert insert into the database all variables
         * @param pkg is the name of the package to insert
         * @param step is the build_step to insert
         * @return if insert routine it's ok will return true otherwise false
         */
        //bool cl_insert(const QString &pkg, const build_step &step);
        /**
         * @brief build_fetch check if there are build_step on the database, if yes it will put all data on values list
         * @param pkg is the name of the package
         * @return if package don't exist or something is wrong, it will return a nullptr
         */
        //QList<build_step *> *cl_fetch(const QString &pkg);
        /**
         * @brief build_exists check if the build_step is on the database
         * @param pkg is the name of the package to find
         * @param step is the build_step to find
         * @return if package don't exist it will return false, otherwise true
         */
        //bool cl_exists(const QString &pkg, const build_step &step);
        /**
         * @brief build_remove remove the build_step on the database
         * @param pkg is the name of the package to find
         * @param step is the build_step to find
         * @return if step_build don't exist it will return false, otherwise true
         */
        //bool cl_remove(const QString &pkg, const build_step &step);
        QSqlDatabase* m_db;
};

#endif // DATABASE_H
