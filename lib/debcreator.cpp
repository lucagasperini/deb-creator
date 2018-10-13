#include "debcreator.h"

#include <QFile>
#include <QTextStream>

debcreator::debcreator(QObject *parent) :
        QObject(parent),
        m_process(new multiprocess),
        m_pkg(new package),
        m_changelog(new changelog(m_pkg)),
        m_db(new database)
{
        QDir dir;
        dir = DEB_CREATOR_LOCAL;
        if(!dir.exists())
                dir.mkdir(DEB_CREATOR_LOCAL);
        dir = DEB_CREATOR_TMP;
        if(!dir.exists())
                dir.mkdir(DEB_CREATOR_TMP);
        dir = DEB_CREATOR_SRC;
        if(!dir.exists())
                dir.mkdir(DEB_CREATOR_SRC);
        dir = DEB_CREATOR_PKG;
        if(!dir.exists())
                dir.mkdir(DEB_CREATOR_PKG);
}
