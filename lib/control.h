#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>

class control : public QObject
{
        Q_OBJECT
public:
        explicit control(QObject *parent = nullptr);
        /**
         * @brief generate generate a control file from variables in this class
         * @return return the control text
         */
        bool generate();
        /**
         * @brief package create the package using QProcess to dpkg
         * @param control is the control text
         * @return return the text of input stream from dpkg process
         */
        bool save();
        /**
         * @brief db_insert insert into the database all variables in this class
         * @return if insert routine it's ok will return true otherwise false
         */
        bool db_insert();
        QStringList db_fetch();
        /**
         * @brief db_fetch check if the package is on the database, if yes it will put all data into the variables in this class.
         * @param pkg is the name of the package to find
         * @return if package don't exist or something is wrong, it will return false, otherwise true
         */
        bool db_fetch(const QString &pkg);
        /**
         * @brief db_exists check if the package is on the database
         * @param pkg is the name of the package to find
         * @return if package don't exist it will return false, otherwise true
         */
        bool db_exists(const QString &pkg);

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

        QString m_text;
};

#endif // CONTROL_H
