#include "control.h"
#include "define.h"

#include <QFile>
#include <QDir>
#include <QTextStream>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>

#ifdef QT_DEBUG
#include <QDebug>
#include <QtSql/QSqlError>
#endif

control::control(QObject *parent) : QObject(parent)
{

}
bool control::generate()
{
        QString offset;

        offset += ("Package: " + debcreator::m_package);
        offset += ("\nMaintainer: " + m_maintainer);
        offset += ("\nUploaders: " + m_uploaders);
        offset += ("\nVersion: " + debcreator::m_version);
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

        m_text = offset;
        return true;
}

bool control::save()
{
        QTextStream out;
        QDir debian_dir(debcreator::m_dir + "/DEBIAN/");
        if(!debian_dir.exists())
                debian_dir.mkdir(debcreator::m_dir + "/DEBIAN/");

        QFile control_file(debcreator::m_dir + "/DEBIAN/control");

        control_file.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&control_file);

        out << m_text;
        out << "\n";

        control_file.flush();
        control_file.close();

        return true;
}

bool control::db_insert()
{
        QSqlQuery* query = new QSqlQuery(*debcreator::m_db);

        if(!debcreator::m_db->tables().contains(DB_PACKAGE_TABLE))
                if(!query->exec(DB_PACKAGE_CREATE))
                        return false;

        if(!db_exists(debcreator::m_package))
                query->prepare(DB_PACKAGE_INSERT);
        else
                query->prepare(DB_PACKAGE_UPDATE);

        query->bindValue(":name", debcreator::m_package);
        query->bindValue(":maintainer", m_maintainer);
        query->bindValue(":uploader", m_uploaders);
        query->bindValue(":version", debcreator::m_version);
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

bool control::db_fetch(const QString &pkg)
{
        QSqlQuery* query = new QSqlQuery(*debcreator::m_db);

        query->prepare(QStringLiteral("SELECT * FROM package WHERE name = :name"));
        query->bindValue(":name", pkg);

        if(!query->exec()) {
#ifdef QT_DEBUG
                qDebug() << query->lastQuery() << query->lastError().text();
#endif
                query->finish();
                return false;
        }


        debcreator::m_package = pkg;
        while (query->next()) {
        m_maintainer = query->value(query->record().indexOf("maintainer")).toString();
        m_uploaders = query->value(query->record().indexOf("uploader")).toString();
        debcreator::m_version = query->value(query->record().indexOf("version")).toString();
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

bool control::db_exists(const QString &pkg)
{
        QSqlQuery* query = new QSqlQuery(*debcreator::m_db);

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
