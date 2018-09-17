#include "debcreator.h"

#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>

#ifdef QT_DEBUG
#include <QDebug>
#include <QtSql/QSqlError>
#endif

debcreator::debcreator(const QString &file_db, QObject *parent) : QObject(parent)
{
        m_db = new QSqlDatabase(QSqlDatabase::addDatabase(QSL("QSQLITE"), QSL("deb-creator-socket")));
        m_db->setDatabaseName(file_db);
        m_db->open();
}

QString debcreator::control()
{
        QString offset;

        offset += QSL("Package: ") + m_package;
        offset += QSL("\nMaintainer: ") + m_maintainer;
        offset += QSL("\nUploaders: ") + m_uploaders;
        offset += QSL("\nVersion: ") + m_version;
        offset += QSL("\nHomepage: ") + m_homepage;
        offset += QSL("\nSource: ") + m_source;
        offset += QSL("\nArchitecture: ") + m_arch;

        if (!m_depends.isEmpty())
                offset += QSL("\nDepends: ") + m_depends;

        offset += QSL("\nReplace: ") + m_replace;
        offset += QSL("\nSection: ") + m_section;
        offset += QSL("\nDescription: ") + m_desc_title;
        if (m_desc_body != "")
                offset += QSL("\n             ") + m_desc_body;

        return offset;
}

bool debcreator::changelog(const QString &text, const QString &status, const QString &urgency)
{
        m_changelog = (m_package + " (" + m_version + ") " + status + "; urgency=" + urgency + "\n\n" +
                       text + "\n\n" +
                       " -- " + git_fetch_user() + " " + date_fetch());
        return true;
}

QString debcreator::package(const QString& control)
{
        QProcess dpkg(this);
        QTextStream out;
        QDir debian_dir(m_dir.path() + QSL("DEBIAN"));
        if(!debian_dir.exists())
                m_dir.mkdir(QSL("DEBIAN"));

        QFile control_file(m_dir.path() + QSL("/DEBIAN/control"));

        control_file.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&control_file);

        out << control;
        out << "\n";

        control_file.flush();
        control_file.close();

        if(!m_changelog.isEmpty()) {
        QFile changelog_file(m_dir.path() + QSL("/DEBIAN/changelog"));

        changelog_file.open(QIODevice::Append | QIODevice::Text);
        out.setDevice(&changelog_file);

        out << m_changelog;
        out << "\n";

        control_file.flush();
        control_file.close();
        }

        QString cmd = "dpkg -b " + m_dir.path() + " " + m_outputfile;

#ifdef QT_DEBUG
        qDebug() << QSL("Executing: ") << cmd;
#endif

        dpkg.start(cmd, QIODevice::ReadWrite);

        QByteArray data;

        while(dpkg.waitForReadyRead()) {
                data.append(dpkg.readAll());
        }
#ifdef QT_DEBUG
        qDebug() << data;
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
                if(line.startsWith(QSL(" -- "))) {
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

        query->bindValue(QSL(":name"), m_package);
        query->bindValue(QSL(":directory"), m_dir.path());
        query->bindValue(QSL(":output"), m_outputfile);
        query->bindValue(QSL(":maintainer"), m_maintainer);
        query->bindValue(QSL(":uploader"), m_uploaders);
        query->bindValue(QSL(":version"), m_version);
        query->bindValue(QSL(":homepage"), m_homepage);
        query->bindValue(QSL(":source"), m_source);
        query->bindValue(QSL(":arch"), m_arch);
        query->bindValue(QSL(":depend"), m_depends);
        query->bindValue(QSL(":replace"), m_replace);
        query->bindValue(QSL(":section"), m_section);
        query->bindValue(QSL(":title"), m_desc_title);
        query->bindValue(QSL(":body"), m_desc_body);

        bool offset = query->exec();

#ifdef QT_DEBUG
        if(!offset)
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
        query->finish();
        return offset;        QDir dir(m_dir);
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


        m_package = pkg;
        while (query->next()) {
                m_dir = query->value(query->record().indexOf(QSL("directory"))).toString();
                m_outputfile = query->value(query->record().indexOf(QSL("output"))).toString();
                m_maintainer = query->value(query->record().indexOf(QSL("maintainer"))).toString();
                m_uploaders = query->value(query->record().indexOf(QSL("uploader"))).toString();
                m_version = query->value(query->record().indexOf(QSL("version"))).toString();
                m_homepage = query->value(query->record().indexOf(QSL("homepage"))).toString();
                m_source = query->value(query->record().indexOf(QSL("source"))).toString();
                m_arch = query->value(query->record().indexOf(QSL("arch"))).toString();
                m_depends = query->value(query->record().indexOf(QSL("depend"))).toString();
                m_replace = query->value(query->record().indexOf(QSL("replace"))).toString();
                m_section = query->value(query->record().indexOf(QSL("section"))).toString();
                m_desc_title = query->value(query->record().indexOf(QSL("title"))).toString();
                m_desc_body = query->value(query->record().indexOf(QSL("body"))).toString();
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

QByteArray debcreator::compile_make(const QString &argument)
{
        QProcess app(this);
        QDir build_dir(m_dir.path() + QSL("/build"));
        app.setWorkingDirectory(build_dir.path());
        QString cmd = QSL("make DESTDIR=") + m_dir.path() + QSL(" install") + argument;

#ifdef QT_DEBUG
        qDebug() << QSL("Executing: ") << cmd;
#endif
        app.start(cmd, QIODevice::ReadWrite);

        QByteArray data;

        while(app.waitForReadyRead()) {
                data.append(app.readAll());
        }

#ifdef QT_DEBUG
        qDebug() << data;
#endif
        build_dir.removeRecursively();
        return data;
}

QByteArray debcreator::compile(const QString &program, const QString &args)
{
        QProcess app(this);
        app.setWorkingDirectory(m_dir.path() + QSL("/build"));
        QString cmd = program + " " + args;

#ifdef QT_DEBUG
        qDebug() << QSL("Executing: ") << cmd;
#endif

        app.start(cmd, QIODevice::ReadWrite);

        QByteArray data;

        while(app.waitForReadyRead()) {
                data.append(app.readAll());
        }
#ifdef QT_DEBUG
        qDebug() << data;
#endif
        return data;
}

QString debcreator::git_clone(const QString &url)
{
        QProcess git(this);
        git.setWorkingDirectory(m_dir.path());
        git.start(QSL("git clone ") + url + QSL(" build"));
#ifdef QT_DEBUG
        qDebug() << QSL("Executing: git clone ") << url << QSL(" build");
#endif
        git.waitForFinished();

        return m_dir.path() + QSL("/build");
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
