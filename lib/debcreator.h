#ifndef DEBCREATOR_H
#define DEBCREATOR_H

#include "define.h"

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QProcess>
#include <QDateTime>


static QString package_name;
static QString package_version;

static QString package_dir;
static QString package_file;

static QSqlDatabase *package_database;

static QString git_fetch_user()
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

static QString date_fetch()
{
        QDateTime now = QDateTime::currentDateTime();
        return now.toString(QSL("ddd, dd MMM yyyy hh:mm:ss t"));
}




class debcreator : public QObject
{
        Q_OBJECT
public:
        explicit debcreator(const QString &file, QObject *parent = nullptr);

        bool package();

};

#endif // DEBCREATOR_H
