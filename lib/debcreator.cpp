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
        return true;
}
