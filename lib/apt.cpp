#include "apt.h"
#include "define.h"

apt::apt(QObject *parent) : QProcess(parent)
{
        m_cache = new QList<package*>;
}

QStringList apt::search(const QString &text)
{
        QStringList offset;
        QString cmd = APT_SEARCH + text + QSL(" --full");

#ifdef QT_DEBUG
        qDebug() << QSL("Executing: ") << cmd;
#endif

        start(cmd, QIODevice::ReadWrite);

        waitForFinished();
        QString line;
        QByteArray buffer;
        package* pkg;
        while (!atEnd()) {
                line = readLine();
                buffer.append(line);
                if(line == QSL("\n"))
                {
                        pkg = new package(buffer);
                        offset.append(pkg->m_name);
                        m_cache->append(pkg);
                }
        }
        return offset;
}

package* apt::cache(const QString &name)
{
        for(int i = 0; i < m_cache->size(); i++)
                if(m_cache->at(i)->m_name == name)
                        return m_cache->at(i);
}
