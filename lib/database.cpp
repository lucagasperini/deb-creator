#include "database.h"
#include "define.h"

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>

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

        if(!pkg_exists(pkg->m_name))
                query->prepare(DB_PACKAGE_INSERT);
        else
                query->prepare(DB_PACKAGE_UPDATE);

        query->bindValue(PKG_NAME, pkg->m_name);
        query->bindValue(PKG_MAINTAINER, pkg->m_maintainer);
        query->bindValue(PKG_UPLOADER, pkg->m_uploaders);
        query->bindValue(PKG_VERSION, pkg->m_version);
        query->bindValue(PKG_HOMEPAGE, pkg->m_homepage);
        query->bindValue(PKG_SOURCE, pkg->m_source);
        query->bindValue(PKG_ARCH, pkg->m_arch);
        query->bindValue(PKG_DEPEND, pkg->m_depends);
        query->bindValue(PKG_BUILD_DEP, pkg->m_build_dep);
        query->bindValue(PKG_REPLACE, pkg->m_replace);
        query->bindValue(PKG_SECTION, pkg->m_section);
        query->bindValue(PKG_DESC_TITLE, pkg->m_desc_title);
        query->bindValue(PKG_DESC_BODY, pkg->m_desc_body);

        bool offset = query->exec();

#ifdef QT_DEBUG
        if(!offset)
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
        query->finish();
        return offset;
}

QStringList database::pkg_fetch()
{
        QStringList offset;
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT name FROM package"));

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return offset;
        }

        while(query->next())
                offset.append(query->value(0).toString());

        query->finish();
        return offset;
}

package *database::pkg_fetch(const QString &pkg)
{
        package* offset = new package();
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT * FROM package WHERE name = :name"));
        query->bindValue(QSL(":name"), pkg);

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return offset;
        }


        offset->m_name = pkg;
        while (query->next()) {
                offset->m_maintainer = query->value(query->record().indexOf(QSL("maintainer"))).toString();
                offset->m_uploaders = query->value(query->record().indexOf(QSL("uploader"))).toString();
                offset->m_version = query->value(query->record().indexOf(QSL("version"))).toString();
                offset->m_homepage = query->value(query->record().indexOf(QSL("homepage"))).toString();
                offset->m_source = query->value(query->record().indexOf(QSL("source"))).toString();
                offset->m_arch = (arch_t)query->value(query->record().indexOf(QSL("arch"))).toInt();
                offset->m_depends = query->value(query->record().indexOf(QSL("depend"))).toString();
                offset->m_build_dep = query->value(query->record().indexOf(QSL("build_dep"))).toString();
                offset->m_replace = query->value(query->record().indexOf(QSL("replace"))).toString();
                offset->m_section = query->value(query->record().indexOf(QSL("section"))).toString();
                offset->m_desc_title = query->value(query->record().indexOf(QSL("title"))).toString();
                offset->m_desc_body = query->value(query->record().indexOf(QSL("body"))).toString();
        }

        query->finish();
        return offset;
}

bool database::pkg_exists(const QString &pkg)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT EXISTS(SELECT 1 FROM package WHERE name=:name)"));
        query->bindValue(":name", pkg);

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

bool database::pkg_remove(const QString &pkg)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("DELETE FROM package WHERE name=:name"));
        query->bindValue(":name", pkg);

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return false;
        }
        return true;
}
