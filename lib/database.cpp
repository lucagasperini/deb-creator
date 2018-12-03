#include "database.h"
#include "define.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>

#include <QVariant>

#ifdef QT_DEBUG
#include <QtSql/QSqlError>
#endif

database::database(QObject *parent) : QObject(parent)
{
        m_db = new QSqlDatabase(QSqlDatabase::addDatabase(QSL("QSQLITE"), QSL("deb-creator-socket")));
        m_db->setDatabaseName(DEB_CREATOR_DB);
        m_db->open();
}

database::database(const QString &file_db, QObject *parent) : QObject(parent)
{
        m_db = new QSqlDatabase(QSqlDatabase::addDatabase(QSL("QSQLITE"), QSL("deb-creator-socket")));
        m_db->setDatabaseName(file_db);
        m_db->open();
}

database::database(const QString &file_db, const QString &socket, QObject *parent) : QObject(parent)
{
        m_db = new QSqlDatabase(QSqlDatabase::addDatabase(QSL("QSQLITE"), socket));
        m_db->setDatabaseName(file_db);
        m_db->open();
}


bool database::pkg_insert(const package *pkg)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        if(!m_db->tables().contains(DB_PACKAGE_TABLE))
                if(!query->exec(DB_PACKAGE_CREATE))
                        return false;

        if(!pkg_exists(pkg->m_id))
                query->prepare(DB_PACKAGE_INSERT);
        else
                query->prepare(DB_PACKAGE_UPDATE);

        query->bindValue(PKG_ID, QVariant(pkg->m_id));
        query->bindValue(PKG_NAME, QVariant(pkg->m_name));
        query->bindValue(PKG_MAINTAINER, QVariant(pkg->m_maintainer));
        query->bindValue(PKG_UPLOADER, QVariant(pkg->m_uploaders));
        query->bindValue(PKG_VERSION, QVariant(pkg->m_version));
        query->bindValue(PKG_HOMEPAGE, QVariant(pkg->m_homepage));
        query->bindValue(PKG_SOURCE, QVariant(pkg->m_source));
        query->bindValue(PKG_ARCH, QVariant(pkg->m_arch));
        query->bindValue(PKG_DEPEND, QVariant(pkg->m_depends));
        query->bindValue(PKG_BUILD_DEP, QVariant(pkg->m_build_dep));
        query->bindValue(PKG_REPLACE, QVariant(pkg->m_replace));
        query->bindValue(PKG_SECTION, QVariant(pkg->m_section));
        query->bindValue(PKG_DESC_TITLE, QVariant(pkg->m_desc_title));
        query->bindValue(PKG_DESC_BODY, QVariant(pkg->m_desc_body));

        bool offset = query->exec();

#ifdef QT_DEBUG
        if(!offset)
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
        query->finish();
        return offset;
}

package_list *database::pkg_fetch()
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT * FROM package"));

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return nullptr;
        }

        package_list* offset = new package_list;
        package* buffer;

        while(query->next()) {
                buffer = new package;
                buffer->m_id = query->value(QSL("id")).toInt();
                buffer->m_name = query->value(QSL("name")).toString();
                buffer->m_maintainer = query->value(QSL("maintainer")).toString();
                buffer->m_uploaders = query->value(QSL("uploader")).toString();
                buffer->m_version = query->value(QSL("version")).toString();
                buffer->m_homepage = query->value(QSL("homepage")).toString();
                buffer->m_source = query->value(QSL("source")).toString();
                buffer->m_arch = (arch_t)query->value(QSL("arch")).toInt();
                buffer->m_depends = query->value(QSL("depend")).toString();
                buffer->m_build_dep = query->value(QSL("build_dep")).toString();
                buffer->m_replace = query->value(QSL("replace")).toString();
                buffer->m_section = query->value(QSL("section")).toString();
                buffer->m_desc_title = query->value(QSL("title")).toString();
                buffer->m_desc_body = query->value(QSL("body")).toString();
                offset->append(buffer);
        }

        query->finish();
        return offset;
}

bool database::pkg_exists(int id)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT EXISTS(SELECT 1 FROM package WHERE id=:id)"));
        query->bindValue(PKG_ID, QVariant(id));

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return false;
        }

        if(query->next())
                return query->value(0).toBool();
}

bool database::pkg_remove(int id)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("DELETE FROM package WHERE id=:id"));
        query->bindValue(PKG_ID, QVariant(id));

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return false;
        }
        return true;
}


bool database::build_insert(const build_step *step)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        if(!m_db->tables().contains(DB_BUILD_TABLE))
                if(!query->exec(DB_BUILD_CREATE))
                        return false;

        query->prepare(DB_BUILD_INSERT);
        query->bindValue(BUILD_PKG, QVariant(step->m_pkg));
        query->bindValue(BUILD_PROGRAM, QVariant(step->m_app));
        query->bindValue(BUILD_ARG, QVariant(step->m_arg));
        query->bindValue(BUILD_DIR, QVariant(step->m_dir));

        bool offset = query->exec();

#ifdef QT_DEBUG
        if(!offset)
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
        query->finish();
        return offset;
}

bool database::build_update(int id, const build_step *step)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        if(!m_db->tables().contains(DB_BUILD_TABLE))
                if(!query->exec(DB_BUILD_CREATE))
                        return false;

        query->prepare(DB_BUILD_UPDATE);
        query->bindValue(BUILD_ID, QVariant(id));
        query->bindValue(BUILD_PKG, QVariant(step->m_pkg));
        query->bindValue(BUILD_PROGRAM, QVariant(step->m_app));
        query->bindValue(BUILD_ARG, QVariant(step->m_arg));
        query->bindValue(BUILD_DIR, QVariant(step->m_dir));

        bool offset = query->exec();

#ifdef QT_DEBUG
        if(!offset)
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
        query->finish();
        return offset;
}

list_build_step* database::build_fetch(int pkg)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT * FROM build WHERE pkg=:pkg;"));
        query->bindValue(BUILD_PKG, pkg);

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return nullptr;
        }

        list_build_step* offset = new list_build_step;
        build_step *buffer;

        while (query->next()) {
                buffer = new build_step;
                buffer->m_id = query->value(QSL("id")).toInt();
                buffer->m_pkg = pkg;
                buffer->m_app = query->value(QSL("program")).toString();
                buffer->m_arg = query->value(QSL("argument")).toString();
                buffer->m_dir = query->value(QSL("directory")).toString();
                offset->append(buffer);
        }

        query->finish();
        return offset;
}


bool database::build_remove(int id)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("DELETE FROM build WHERE id=:id"));
        query->bindValue(BUILD_ID, QVariant(id));

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return false;
        }
        return true;
}
bool database::cl_insert(const changelog *cl)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        if(!m_db->tables().contains(DB_CL_TABLE))
                if(!query->exec(DB_CL_CREATE))
                        return false;

        query->prepare(DB_CL_INSERT);
        query->bindValue(CL_PKG, QVariant(cl->m_pkg->m_id));
        query->bindValue(CL_TEXT, QVariant(cl->m_text));
        query->bindValue(CL_VERSION, QVariant(cl->m_version));
        query->bindValue(CL_STATUS, QVariant(cl->m_status));
        query->bindValue(CL_URGENCY, QVariant(cl->m_urgency));

        bool offset = query->exec();

#ifdef QT_DEBUG
        if(!offset)
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
        query->finish();
        return offset;
}

bool database::cl_update(int id, const changelog *cl)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        if(!m_db->tables().contains(DB_CL_TABLE))
                if(!query->exec(DB_CL_CREATE))
                        return false;

        query->prepare(DB_CL_UPDATE);
        query->bindValue(CL_ID, QVariant(id));
        query->bindValue(CL_PKG, QVariant(cl->m_pkg->m_id));
        query->bindValue(CL_TEXT, QVariant(cl->m_text));
        query->bindValue(CL_VERSION, QVariant(cl->m_version));
        query->bindValue(CL_STATUS, QVariant(cl->m_status));
        query->bindValue(CL_URGENCY, QVariant(cl->m_urgency));

        bool offset = query->exec();

#ifdef QT_DEBUG
        if(!offset)
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
        query->finish();
        return offset;
}

changelog_list *database::cl_fetch(const package* pkg)
{
        changelog_list* offset = nullptr;
        QSqlQuery* query = new QSqlQuery(*m_db);

        if(!m_db->tables().contains(DB_CL_TABLE))
                if(!query->exec(DB_CL_CREATE))
                        return nullptr;

        query->prepare(QSL("SELECT * FROM changelog WHERE pkg=:pkg"));
        query->bindValue(CL_PKG, pkg->m_id);

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return offset;
        }

        offset = new changelog_list;
        changelog *buffer;


        while (query->next()) {
                buffer = new changelog;
                buffer->m_id = query->value(QSL("id")).toInt();
                buffer->m_pkg = pkg;
                buffer->m_text = query->value(QSL("text")).toByteArray();
                buffer->m_version = query->value(QSL("version")).toString();
                buffer->m_status = query->value(QSL("status")).toString();
                buffer->m_urgency = query->value(QSL("urgency")).toString();
                offset->append(buffer);
        }

        query->finish();
        return offset;
}

int database::cl_find(int pkg, const QByteArray &cl)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT id FROM changelog WHERE pkg=:pkg AND text=:text"));
        query->bindValue(CL_PKG, QVariant(pkg));
        query->bindValue(CL_TEXT, QVariant(cl));

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return 0;
        }

        if(query->next())
                return query->value(0).toInt();
}

bool database::cl_remove(int id)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("DELETE FROM changelog WHERE id=:id"));
        query->bindValue(CL_ID, QVariant(id));

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return false;
        }
        return true;
}
