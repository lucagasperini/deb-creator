#include "debcreator.h"

#include <QProcess>
#include <QDateTime>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>

#ifdef QT_DEBUG
#include <QDebug>
#include <QtSql/QSqlError>
#endif

debcreator::debcreator(const QString &file, QObject *parent) : QObject(parent)
{
        package_database = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "deb-creator-socket"));
        package_database->setDatabaseName(file);
        package_database->open();
}

bool debcreator::package()
{
        QProcess dpkg(this);

        QString cmd = "dpkg -b " + package_dir + " " + package_file;

#ifdef QT_DEBUG
        qDebug() << "Executing: " << cmd;
#endif

        dpkg.start(cmd, QIODevice::ReadWrite);

        QByteArray data;

        while(dpkg.waitForReadyRead()) {
                data.append(dpkg.readAll());
        }
#ifdef QT_DEBUG
        qDebug() << data.data();
#endif
        return data.data();
}

QStringList debcreator::fetch_changelog(const QString &file)
{
        if(file.isEmpty())
                return QStringList();

        QStringList offset;
        QString buffer;
        QFile changelog_file(file);

        if(!changelog_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return QStringList();
        }

        QTextStream in(&changelog_file);

        while (!in.atEnd()) {
                QString line = in.readLine();
                buffer.append(line);
                if(line.startsWith(" -- ")) {
                        offset.append(buffer);
                        buffer.clear();
                }
        }
        changelog_file.close();

        return offset;
}

bool debcreator::db_insert()
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        if(!m_db->tables().contains(DB_PACKAGE_TABLE))
                if(!query->exec(DB_PACKAGE_CREATE))
                        return false;

        if(!db_exists(m_package))
                query->prepare(DB_PACKAGE_INSERT);
        else
                query->prepare(DB_PACKAGE_UPDATE);

        query->bindValue(":name", m_package);
        query->bindValue(":maintainer", m_maintainer);
        query->bindValue(":uploader", m_uploaders);
        query->bindValue(":version", m_version);
        query->bindValue(":homepage", m_homepage);
        query->bindValue(":source", m_source);
        query->bindValue(":arch", m_arch);
        query->bindValue(":depend", m_depends);
        query->bindValue(":replace", m_replace);
        query->bindValue(":section", m_section);
        query->bindValue(":title", m_desc_title);
        query->bindValue(":body", m_desc_body);

        bool offset = query->exec();

#ifdef QT_DEBUG
        if(!offset)
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
        query->finish();
        return offset;
}

bool debcreator::db_fetch(const QString &pkg)
{
        QSqlQuery* query = new QSqlQuery(*m_db);

        query->prepare(QSL("SELECT * FROM package WHERE name = :name"));
        query->bindValue(":name", pkg);

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return false;
        }


        m_package = pkg;
        while (query->next()) {
        m_maintainer = query->value(query->record().indexOf("maintainer")).toString();
        m_uploaders = query->value(query->record().indexOf("uploader")).toString();
        m_version = query->value(query->record().indexOf("version")).toString();
        m_homepage = query->value(query->record().indexOf("homepage")).toString();
        m_source = query->value(query->record().indexOf("source")).toString();
        m_arch = query->value(query->record().indexOf("arch")).toString();
        m_depends = query->value(query->record().indexOf("depend")).toString();
        m_replace = query->value(query->record().indexOf("replace")).toString();
        m_section = query->value(query->record().indexOf("section")).toString();
        m_desc_title = query->value(query->record().indexOf("title")).toString();
        m_desc_body = query->value(query->record().indexOf("body")).toString();
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


QString debcreator::git_fetch_user()
{
        QProcess git;
        QString name;
        QString mail;

        git.start(QSL("git config --get user.name"));
        git.waitForReadyRead();
        name = git.readAll().trimmed();

        git.close();

        git.start(QSL("git config --get user.email"));
        git.waitForReadyRead();
        mail = git.readAll().trimmed();

        git.close();

        return name + " " + "<" + mail + ">";
}

QString debcreator::date_fetch()
{
        QDateTime now = QDateTime::currentDateTime();
        return now.toString(QSL("ddd, dd MMM yyyy hh:mm:ss t"));
}
