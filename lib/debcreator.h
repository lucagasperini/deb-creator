#ifndef DEBCREATOR_H
#define DEBCREATOR_H

#include <QObject>
#include <QtSql/QSqlDatabase>

class debcreator : public QObject
{
        Q_OBJECT
public:
        explicit debcreator(const QString &file, QObject *parent = nullptr);
        QString control();
        QString package(const QString& control);

        bool db_insert();
        bool db_check(const QString &pkg);

        QSqlDatabase* m_db;

        QString m_package;
        QString m_version;
        QString m_arch;
        QString m_depends;
        QString m_maintainer;
        QString m_desc_title;
        QString m_desc_body;
        QString m_homepage;
        QString m_uploaders;
        QString m_section;
        QString m_source;
        QString m_replace;

        QString m_control;
        QString m_dir;
        QString m_outputfile;
};

#endif // DEBCREATOR_H
