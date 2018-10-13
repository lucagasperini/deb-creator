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

        multiprocess* m_process;
        package *m_pkg;
        changelog *m_changelog;
        database *m_db;
};

#endif // DEBCREATOR_H
