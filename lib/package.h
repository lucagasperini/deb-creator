#ifndef PACKAGE_H
#define PACKAGE_H

#include <QObject>
#include <QDir>

enum arch_t { error = -1, all = 0, i386 = 1, amd64 = 2 };

class package : public QObject
{
        Q_OBJECT
public:
        explicit package(QObject *parent = nullptr);
        explicit package(const package &pkg);
        explicit package(const QString &control);

        bool is_empty();
        /**
         * @brief control generate a control file from variables in this class
         * @return return the control text
         */
        QByteArray control();
        /**
         * @brief calc_size calculate the size of package directory
         * @param _dir root directory of the package
         * @return integer 64 bit of size in byte
         */
        static qint64 calc_size(const QString &_dir);
        static QString architecture_name(const arch_t &arch);
        static arch_t architecture_value(const QString &arch);

        QString m_name;
        QString m_version;
        arch_t m_arch;
        qint64 m_size;
        QString m_depends;
        QString m_maintainer;
        QString m_desc_title;
        QString m_desc_body;
        QString m_homepage;
        QString m_uploaders;
        QString m_section;
        QString m_source;
        QString m_replace;

        QDir m_dir;
};

#endif // PACKAGE_H
