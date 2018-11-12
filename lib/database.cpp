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

QMap<int, QString> *database::pkg_fetch()
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT id,name FROM package"));

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return nullptr;
        }

        QMap<int, QString>* offset = new QMap<int, QString>;

        while(query->next()) {
                offset->insert(query->value(0).toInt(), query->value(1).toString());
        }

        query->finish();
        return offset;
}

package *database::pkg_fetch(int id)
{
        package* offset = new package();
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT * FROM package WHERE id = :id"));
        query->bindValue(PKG_ID, QVariant(id));

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return offset;
        }

        offset->m_id = id;
        while (query->next()) {
                offset->m_name = query->value(QSL("name")).toString();
                offset->m_maintainer = query->value(QSL("maintainer")).toString();
                offset->m_uploaders = query->value(QSL("uploader")).toString();
                offset->m_version = query->value(QSL("version")).toString();
                offset->m_homepage = query->value(QSL("homepage")).toString();
                offset->m_source = query->value(QSL("source")).toString();
                offset->m_arch = (arch_t)query->value(QSL("arch")).toInt();
                offset->m_depends = query->value(QSL("depend")).toString();
                offset->m_build_dep = query->value(QSL("build_dep")).toString();
                offset->m_replace = query->value(QSL("replace")).toString();
                offset->m_section = query->value(QSL("section")).toString();
                offset->m_desc_title = query->value(QSL("title")).toString();
                offset->m_desc_body = query->value(QSL("body")).toString();
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


bool database::build_insert(const QString &pkg, const build_step &step)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        if(!m_db->tables().contains(DB_BUILD_TABLE))
                if(!query->exec(DB_BUILD_CREATE))
                        return false;

        if(!build_exists(pkg, step))
                query->prepare(DB_BUILD_INSERT);
        else
                query->prepare(DB_BUILD_UPDATE); /* FIXME: DIDN'T WORK */

        query->bindValue(BUILD_PROGRAM, QVariant(step.program));
        query->bindValue(BUILD_ARG, QVariant(step.argument));
        query->bindValue(BUILD_DIR, QVariant(step.directory));
        query->bindValue(BUILD_PKG, QVariant(pkg));

        bool offset = query->exec();

#ifdef QT_DEBUG
        if(!offset)
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
        query->finish();
        return offset;
}

QList<build_step*>* database::build_fetch(const QString &pkg)
{
        QList<build_step*>* offset = nullptr;
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT * FROM build WHERE pkg=:pkg"));
        query->bindValue(BUILD_PKG, pkg);

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return offset;
        }

        offset = new QList<build_step*>;
        build_step *buffer;


        while (query->next()) {
                buffer = new build_step;
                buffer->program = query->value(QSL("program")).toString();
                buffer->argument = query->value(QSL("argument")).toString();
                buffer->directory = query->value(QSL("directory")).toString();
                offset->append(buffer);
        }

        query->finish();
        return offset;
}

bool database::build_exists(const QString &pkg, const build_step &step)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT EXISTS(SELECT 1 FROM build WHERE program=:program AND argument=:argument AND directory=:directory AND pkg=:pkg)"));
        query->bindValue(BUILD_PROGRAM, QVariant(step.program));
        query->bindValue(BUILD_ARG, QVariant(step.argument));
        query->bindValue(BUILD_DIR, QVariant(step.directory));
        query->bindValue(BUILD_PKG, QVariant(pkg));

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

bool database::build_remove(const QString &pkg, const build_step &step)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("DELETE FROM build WHERE program=:program AND argument=:argument AND directory=:directory AND pkg=:pkg"));
        query->bindValue(BUILD_PROGRAM, QVariant(step.program));
        query->bindValue(BUILD_ARG, QVariant(step.argument));
        query->bindValue(BUILD_DIR, QVariant(step.directory));
        query->bindValue(BUILD_PKG, QVariant(pkg));

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return false;
        }
        return true;
}
