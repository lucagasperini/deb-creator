#include "debcreator.h"

#include <QFile>
#include <QTextStream>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>

debcreator::debcreator(const QString &file_db, QObject *parent) : QObject(parent)
{
        QDir local(DEB_CREATOR_LOCAL);
        if(!local.exists())
                local.mkdir(DEB_CREATOR_LOCAL);
        QDir tmp(DEB_CREATOR_TMP);
        if(!tmp.exists())
                tmp.mkdir(DEB_CREATOR_TMP);
        QDir src(DEB_CREATOR_SRC);
        if(!src.exists())
                src.mkdir(DEB_CREATOR_SRC);

        m_db = new QSqlDatabase(QSqlDatabase::addDatabase(QSL("QSQLITE"), QSL("deb-creator-socket")));

        if(file_db.isEmpty())
                m_db->setDatabaseName(DEB_CREATOR_DB);
        else
                m_db->setDatabaseName(file_db);

        m_db->open();

        m_process = new multiprocess;
        m_pkg = new package;
        m_changelog = new changelog;
        m_git = new git;
}


QByteArray debcreator::pkg_create(const QByteArray& control, const QString &outputfile)
{
        QProcess dpkg(this);
        QTextStream out;
        QDir debian_dir(m_pkg->m_dir.path() + QSL("DEBIAN"));
        if(!debian_dir.exists())
                m_pkg->m_dir.mkdir(QSL("DEBIAN"));

        QFile control_file(m_pkg->m_dir.path() + QSL("/DEBIAN/control"));

        control_file.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&control_file);

        out << control;
        out << "\n";

        control_file.flush();
        control_file.close();

        QString cmd;
        if(outputfile.isEmpty())
                cmd = "dpkg -b " + m_pkg->m_dir.path() + " " + QDir::homePath() + "/" + gen_outputfile();
        else
                cmd = "dpkg -b " + m_pkg->m_dir.path() + " " + outputfile;

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

QString debcreator::gen_outputfile()
{
        return m_pkg->m_name + "_" + m_pkg->m_version + ".deb";
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

        query->bindValue(QSL(":name"), m_pkg->m_name);
        query->bindValue(QSL(":directory"), m_pkg->m_dir.path());
        query->bindValue(QSL(":maintainer"), m_pkg->m_maintainer);
        query->bindValue(QSL(":uploader"), m_pkg->m_uploaders);
        query->bindValue(QSL(":version"), m_pkg->m_version);
        query->bindValue(QSL(":homepage"), m_pkg->m_homepage);
        query->bindValue(QSL(":source"), m_pkg->m_source);
        query->bindValue(QSL(":arch"), m_pkg->m_arch);
        query->bindValue(QSL(":depend"), m_pkg->m_depends);
        query->bindValue(QSL(":replace"), m_pkg->m_replace);
        query->bindValue(QSL(":section"), m_pkg->m_section);
        query->bindValue(QSL(":title"), m_pkg->m_desc_title);
        query->bindValue(QSL(":body"), m_pkg->m_desc_body);

        bool offset = query->exec();

#ifdef QT_DEBUG
        if(!offset)
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
        query->finish();
        return offset;
        QDir dir(m_pkg->m_dir);
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
                m_pkg->m_dir = query->value(query->record().indexOf(QSL("directory"))).toString();
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
