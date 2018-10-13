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

        multiprocess* m_process;
        package *m_pkg;
        changelog *m_changelog;
        database *m_db;
};

#endif // DEBCREATOR_H
