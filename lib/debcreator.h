#ifndef DEBCREATOR_H
#define DEBCREATOR_H

#include "multiprocess.h"
#include "define.h"
#include "package.h"
#include "changelog.h"
#include "git.h"
#include "filesystem.h"
#include "database.h"

class debcreator : public QObject
{
        Q_OBJECT
public:
        /**
         * @brief debcreator default constructor
         * @param parent default qt parent
         */
        explicit debcreator(QObject *parent = nullptr);
        /**
         * @brief package create the package using QProcess to dpkg
         * @param control is the control text
         * @param outputfile the path where package is created
         */
        QByteArray pkg_create(const QByteArray &control, const QString &outputfile);
        /**
         * @brief build_dir current build directory for stored package
         * @return string of build directory path
         */
        QString build_dir();
        /**
         * @brief file_write write data in selected filename
         * @param filename target file
         * @param data binary data to write
         * @return if cannot write in target file it will return false, otherwise true
         */
        static bool file_write(const QString &filename, const QString &data);
        /**
         * @brief file_read read binary data from target file
         * @param filename file where data is stored
         * @return data from target file
         */
        static QByteArray file_read(const QString &filename);

        multiprocess* m_process;
        package *m_pkg;
        changelog *m_changelog;
        database *m_db;
};

#endif // DEBCREATOR_H
