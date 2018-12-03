#ifndef DATABASE_H
#define DATABASE_H

#include "package_list.h"
#include "list_build_step.h"
#include "changelog_list.h"

#include <QObject>
#include <QtSql/QSqlDatabase>

class database : public QObject
{
        Q_OBJECT
public:
        explicit database(QObject *parent = nullptr);
        explicit database(const QString &file_db, QObject *parent = nullptr);
        explicit database(const QString &file_db, const QString &socket = "deb-creator-socket", QObject *parent = nullptr);

        /**
         * @brief pkg_insert insert into the database all variables
         * @param pkg is the package to insert
         * @return if insert routine it's ok will return true otherwise false
         */
        bool pkg_insert(const package *pkg);
        /**
         * @brief pkg_fetch fetch all package from database
         * @return list of packages name of database
         */
        package_list* pkg_fetch();
        /**
         * @brief pkg_exists check if the package is on the database
         * @param id is the index of the package to find
         * @return if package don't exist it will return false, otherwise true
         */
        bool pkg_exists(int id);
        /**
         * @brief pkg_remove remove the package on the database
         * @param id is the index of the package to remove
         * @return if package don't exist it will return false, otherwise true
         */
        bool pkg_remove(int id);
        /**
         * @brief build_insert insert into the database all variables
         * @param pkg is the name of the package to insert
         * @param step is the build_step to insert
         * @return if insert routine it's ok will return true otherwise false
         */
        bool build_insert(const build_step &step);
        /**
         * @brief build_insert insert into the database all variables
         * @param pkg is the name of the package to insert
         * @param step is the build_step to insert
         * @return if insert routine it's ok will return true otherwise false
         */
        bool build_update(int id, const build_step &step);
        /**
         * @brief build_fetch check if there are build_step on the database, if yes it will put all data on values list
         * @param pkg is the name of the package
         * @return if package don't exist or something is wrong, it will return a nullptr
         */
        list_build_step *build_fetch(int pkg);
        /**
         * @brief build_remove remove the build_step on the database
         * @param pkg is the name of the package to find
         * @param step is the build_step to find
         * @return if step_build don't exist it will return false, otherwise true
         */
        bool build_remove(int id);
        /**
         * @brief cl_insert insert into the database all variables
         * @param cl is the changelog to insert
         * @return if insert routine it's ok will return true otherwise false
         */
        bool cl_insert(const changelog &cl);
        /**
         * @brief cl_update update a changelog into database
         * @param id is the id of changelog to update
         * @param cl is the changelog to update
         * @return if insert routine it's ok will return true otherwise false
         */
        bool cl_update(int id, const changelog &cl);
        /**
         * @brief cl_fetch check if there are changelog on the database, if yes it will put all data on values list
         * @param pkg is the name of the package
         * @return if package don't exist or something is wrong, it will return a nullptr
         */
        changelog_list *cl_fetch(const package *pkg);
        /**
         * @brief cl_exists find id of the changelog on the database
         * @param pkg is the id package of changelog
         * @param text is the changelog text
         * @return id of the changelog
         */
        int cl_find(int pkg, const QByteArray &cl);
        /**
         * @brief cl_remove remove the build_step on the database
         * @param pkg is the name of the package to find
         * @param step is the build_step to find
         * @return if step_build don't exist it will return false, otherwise true
         */
        bool cl_remove(int id);
        QSqlDatabase* m_db;
};

#endif // DATABASE_H
