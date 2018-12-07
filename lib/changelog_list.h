#ifndef CHANGELOG_LIST_H
#define CHANGELOG_LIST_H

#include "changelog.h"

#include <QList>

class changelog_list : public QObject, public QList<changelog*>
{
public:
        changelog_list(QObject *parent = nullptr);
        /**
         * @brief titles get all titles from changelogs
         * @return string list with titles
         */
        QStringList titles() const;
        /**
         * @brief text get all formatted text from changelogs
         * @return byte array ready to write into changelog file
         */
        QByteArray text() const;
};

#endif // CHANGELOG_LIST_H
