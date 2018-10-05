#ifndef APT_H
#define APT_H

#include "package.h"

#include <QObject>
#include <QList>
#include <QProcess>

#include <apt-pkg/cachefile.h>
#include <apt-pkg/pkgcache.h>


class apt : public QProcess
{
        Q_OBJECT
public:
        explicit  apt(QObject *parent = nullptr);
        QStringList search(const QString &text);
        package* cache(const QString &name);
        QList<package*>* m_cache;
};

#endif // APT_H
