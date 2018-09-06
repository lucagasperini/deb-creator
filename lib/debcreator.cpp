#include "debcreator.h"
#include "define.h"

#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>

#ifdef QT_DEBUG
#include <QDebug>
#include <QtSql/QSqlError>
#endif

debcreator::debcreator(const QString &file, QObject *parent) : QObject(parent)
{
        m_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "deb-creator-socket"));
        m_db->setDatabaseName(file);
        m_db->open();
}

QString debcreator::control()
{
        QString offset;

        if (m_package.contains(' ')) {
                QMessageBox::warning(nullptr, "Control file error", "Package name must not contain spaces.");
                return "";
        }

        offset += ("Package: " + m_package);
        offset += ("\nMaintainer: " + m_maintainer);
        offset += ("\nUploaders: " + m_uploaders);
        offset += ("\nVersion: " + m_version);
        offset += ("\nHomepage: " + m_homepage);
        offset += ("\nSource: " + m_source);

        if (m_arch.isEmpty())
                offset += "\nArchitecture: all";
        else
                offset += ("\nArchitecture: " + m_arch);

        if (!m_depends.isEmpty())
                offset += ("\nDepends: " + m_depends);

        offset += ("\nReplace: " + m_replace);
        offset += ("\nSection: " + m_section);
        offset += ("\nDescription: " + m_desc_title);
        if (m_desc_body != "")
                offset += "\n             " + m_desc_body;

        return offset;
}

QString debcreator::package(const QString& control)
{
        QProcess dpkg(this);
        QDir debian_dir(m_dir + "/DEBIAN/");
        debian_dir.mkdir(m_dir + "/DEBIAN/");
        QFile control_file(m_dir + "/DEBIAN/control");

        control_file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&control_file);

        out << control;
        out << "\n";

        control_file.flush();
        control_file.close();

        QString cmd = "dpkg -b " + m_dir + " " + m_outputfile;

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

        query->prepare(QStringLiteral("SELECT * FROM package WHERE name = :name"));
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

        query->prepare(QStringLiteral("SELECT EXISTS(SELECT 1 FROM package WHERE name=:name)"));
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
