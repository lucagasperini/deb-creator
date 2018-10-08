#include "debcreator.h"

#include <QFile>
#include <QTextStream>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>

debcreator::debcreator(const QString &file_db, QObject *parent) : QObject(parent)
{
        QDir dir;
        dir = DEB_CREATOR_LOCAL;
        if(!dir.exists())
                dir.mkdir(DEB_CREATOR_LOCAL);
        dir = DEB_CREATOR_TMP;
        if(!dir.exists())
                dir.mkdir(DEB_CREATOR_TMP);
        dir = DEB_CREATOR_SRC;
        if(!dir.exists())
                dir.mkdir(DEB_CREATOR_SRC);
        dir = DEB_CREATOR_PKG;
        if(!dir.exists())
                dir.mkdir(DEB_CREATOR_PKG);

        m_db = new QSqlDatabase(QSqlDatabase::addDatabase(QSL("QSQLITE"), QSL("deb-creator-socket")));

        if(file_db.isEmpty())
                m_db->setDatabaseName(DEB_CREATOR_DB);
        else
                m_db->setDatabaseName(file_db);

        m_db->open();

        m_process = new multiprocess;
        m_pkg = new package;
        m_changelog = new changelog(m_pkg);
        m_git = new git;
}


QByteArray debcreator::pkg_create(const QByteArray& control, const QString &outputfile)
{
        QProcess dpkg(this);
        QTextStream out;
        QString pkg_root = m_pkg->root();
        QDir debian_dir(pkg_root + QSL("/DEBIAN"));
        if(!debian_dir.exists())
                debian_dir.mkdir(pkg_root + QSL("/DEBIAN"));

        QFile control_file(pkg_root + QSL("/DEBIAN/control"));

        control_file.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&control_file);

        out << control;
        out << "\n";

        control_file.flush();
        control_file.close();

        QString cmd = QSL("dpkg -b ") + pkg_root + QSL(" ") + outputfile;

#ifdef QT_DEBUG
        qDebug() << QSL("Executing: ") << cmd;
#endif

        dpkg.start(cmd, QIODevice::ReadWrite);

        QByteArray data;

        while(dpkg.waitForReadyRead()) {
                data.append(dpkg.readAll());
        }
        return data;
}

bool debcreator::db_insert()
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        if(!m_db->tables().contains(DB_PACKAGE_TABLE))
                if(!query->exec(DB_PACKAGE_CREATE))
                        return false;

        if(!db_exists(m_pkg->m_name))
                query->prepare(DB_PACKAGE_INSERT);
        else
                query->prepare(DB_PACKAGE_UPDATE);

        query->bindValue(PKG_NAME, m_pkg->m_name);
        query->bindValue(PKG_MAINTAINER, m_pkg->m_maintainer);
        query->bindValue(PKG_UPLOADER, m_pkg->m_uploaders);
        query->bindValue(PKG_VERSION, m_pkg->m_version);
        query->bindValue(PKG_HOMEPAGE, m_pkg->m_homepage);
        query->bindValue(PKG_SOURCE, m_pkg->m_source);
        query->bindValue(PKG_ARCH, m_pkg->m_arch);
        query->bindValue(PKG_DEPEND, m_pkg->m_depends);
        query->bindValue(PKG_REPLACE, m_pkg->m_replace);
        query->bindValue(PKG_SECTION, m_pkg->m_section);
        query->bindValue(PKG_DESC_TITLE, m_pkg->m_desc_title);
        query->bindValue(PKG_DESC_BODY, m_pkg->m_desc_body);

        bool offset = query->exec();

#ifdef QT_DEBUG
        if(!offset)
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
        query->finish();
        return offset;
}

QStringList debcreator::db_fetch()
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

bool debcreator::db_fetch(const QString &pkg)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT * FROM package WHERE name = :name"));
        query->bindValue(QSL(":name"), pkg);

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return false;
        }


        m_pkg->m_name = pkg;
        while (query->next()) {
                m_pkg->m_maintainer = query->value(query->record().indexOf(QSL("maintainer"))).toString();
                m_pkg->m_uploaders = query->value(query->record().indexOf(QSL("uploader"))).toString();
                m_pkg->m_version = query->value(query->record().indexOf(QSL("version"))).toString();
                m_pkg->m_homepage = query->value(query->record().indexOf(QSL("homepage"))).toString();
                m_pkg->m_source = query->value(query->record().indexOf(QSL("source"))).toString();
                m_pkg->m_arch = (arch_t)query->value(query->record().indexOf(QSL("arch"))).toInt();
                m_pkg->m_depends = query->value(query->record().indexOf(QSL("depend"))).toString();
                m_pkg->m_replace = query->value(query->record().indexOf(QSL("replace"))).toString();
                m_pkg->m_section = query->value(query->record().indexOf(QSL("section"))).toString();
                m_pkg->m_desc_title = query->value(query->record().indexOf(QSL("title"))).toString();
                m_pkg->m_desc_body = query->value(query->record().indexOf(QSL("body"))).toString();
        }

        query->finish();
        return true;
}

bool debcreator::db_exists(const QString &pkg)
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

bool debcreator::db_remove(const QString &pkg)
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
