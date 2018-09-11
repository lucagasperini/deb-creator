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

QString debcreator::m_package;
QString debcreator::m_version;
QString debcreator::m_dir;
QString debcreator::m_outputfile;
QSqlDatabase *debcreator::m_db;

debcreator::debcreator(const QString &file, QObject *parent) : QObject(parent)
{
        m_db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "deb-creator-socket"));
        m_db->setDatabaseName(file);
        m_db->open();
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

bool debcreator::package()
{
        QProcess dpkg(this);

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
        return true;
}
