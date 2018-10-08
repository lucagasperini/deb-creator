#ifndef APT_H
#define APT_H

#include "package.h"

#include <QObject>
#include <QList>
#include <QProcess>

class apt : public QProcess
{
        Q_OBJECT
public:
        explicit  apt(QObject *parent = nullptr);
        /**
         * @brief search start apt-cache process to search a package
         * @param text string to search
         * @return the list of packages
         */
        QList<package *>* search(const QString &text);
};

#endif // APT_H
