#include "apt.h"
#include "define.h"

apt::apt(QObject *parent) : QProcess(parent)
{
        m_cache = new QList<package*>;
        pkgInitConfig(*_config);
        pkgInitSystem(*_config, _system);
}

QStringList apt::search(const QString &text)
{
        QStringList offset;
        package* buffer;

        pkgCacheFile cache_file;
        pkgCache* cache = cache_file.GetPkgCache();
        //qDebug() << cache->FindPkg("steam", "i386").Name();

        for (pkgCache::PkgIterator pkg = cache->FindPkg(text.toStdString()); !pkg.end(); pkg++) {
                buffer = new package;
                buffer->m_arch = package::architecture_value(pkg.Arch());
                buffer->m_name = pkg.Name();
                buffer->m_version = pkg.CurVersion();
                buffer->m_section = pkg.CurrentVer().Section();

                offset << buffer->m_name;
                m_cache->append(buffer);
        }
        /*        for (pkgCache::PkgFileIterator package = cache->FileBegin(); !package.end(); package++) {
                        offset << package.FileName();
                }*/
        return offset;
}

package* apt::cache(const QString &name)
{
        for(int i = 0; i < m_cache->size(); i++)
                if(m_cache->at(i)->m_name == name)
                        return m_cache->at(i);
}
