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
         * @return the list of package name
         */
        QStringList search(const QString &text);
        /**
         * @brief cache get from memory a pointer of package by name
         * @param name string of the package
         * @return memory pointer of package
         */
        package* cache(const QString &name);
        /**
         * @brief clear remove all package in cache memory
         */
        void clear();

        QList<package*>* m_cache;
};

#endif // APT_H
