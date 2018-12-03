#ifndef CHANGELOG_LIST_H
#define CHANGELOG_LIST_H

#include "changelog.h"

#include <QList>

class changelog_list : public QObject, public QList<changelog*>
{
public:
        changelog_list(QObject *parent = nullptr);
        QStringList titles() const;
        QByteArray text() const;
};

#endif // CHANGELOG_LIST_H
