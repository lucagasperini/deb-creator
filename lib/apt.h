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
         * @brief exec execute apt process with argument and working directory
         * @return output of apt process
         */
        QByteArray exec();
        /**
         * @brief search start apt-cache process to search a package
         * @param text string to search
         * @return the list of packages
         */
        QList<package *>* search(const QString &text);
        /**
         * @brief install install pkglist on system
         * @param pkglist list of packages
         */
        void install(const QStringList &pkglist);
};

#endif // APT_H
