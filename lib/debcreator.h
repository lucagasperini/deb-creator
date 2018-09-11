#ifndef DEBCREATOR_H
#define DEBCREATOR_H

#include <QObject>
#include <QtSql/QSqlDatabase>

class debcreator : public QObject
{
        Q_OBJECT
public:
        explicit debcreator(const QString &file, QObject *parent = nullptr);
        /**
         * @brief git_fetch_user fetch user.email and user.name from git config
         * @return the format from git is user.name <user.email>, empty string if git is not installed or git is not configurated
         */
        static QString git_fetch_user();
        /**
         * @brief date_fetch fetch date in standard format from locale
         * @return the formated string of date
         */
        static QString date_fetch();
        bool package();

        static QSqlDatabase* m_db;

        static QString m_package;
        static QString m_version;

        static QString m_dir;
        static QString m_outputfile;
};

#endif // DEBCREATOR_H
