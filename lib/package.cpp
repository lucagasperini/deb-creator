#include "package.h"
#include "define.h"

package::package(QObject *parent) : QObject(parent)
{

}

package::package(const package &pkg)
{
        m_name = pkg.m_name;
        m_version = pkg.m_version;
        m_arch = pkg.m_arch;
        m_depends = pkg.m_depends;
        m_maintainer = pkg.m_maintainer;
        m_desc_title = pkg.m_desc_title;
        m_desc_body = pkg.m_desc_body;
        m_homepage = pkg.m_homepage;
        m_uploaders = pkg.m_uploaders;
        m_section = pkg.m_section;
        m_source = pkg.m_source;
        m_replace = pkg.m_replace;
}

package::package(const QByteArray &data)
{
        QList<QByteArray> buffer = data.split(':');

        for(int i = 0; i < buffer.size(); i+=2) {
                if(buffer.at(i) == QSL("Package: "))
                        m_name = buffer.at(i+1);
                else if(buffer.at(i) == QSL("Maintainer: "))
                        m_maintainer = buffer.at(i+1);
                else if(buffer.at(i) == QSL("Uploaders: "))
                        m_uploaders = buffer.at(i+1);
                else if(buffer.at(i) == QSL("Version: "))
                        m_version = buffer.at(i+1);
                else if(buffer.at(i) == QSL("Homepage: "))
                        m_homepage = buffer.at(i+1);
                else if(buffer.at(i) == QSL("Source: "))
                        m_source = buffer.at(i+1);
                else if(buffer.at(i) == QSL("Installed-Size: "))
                        m_size = buffer.at(i+1).toInt();
                else if(buffer.at(i) == QSL("Architecture: "))
                        m_arch = package::architecture_value(buffer.at(i+1));
                else if(buffer.at(i) == QSL("Depends: "))
                        m_depends = buffer.at(i+1);
                else if(buffer.at(i) == QSL("Replace: "))
                        m_replace = buffer.at(i+1);
                else if(buffer.at(i) == QSL("Section: "))
                        m_section = buffer.at(i+1);
                else if(buffer.at(i) == QSL("Description: "))
                        m_desc_body = buffer.at(i+1);
        }
}

bool package::is_empty()
{
        return m_name.isEmpty();
}

QByteArray package::control()
{
        QByteArray offset;

        offset += QSL("Package: ") + m_name;
        offset += QSL("\nMaintainer: ") + m_maintainer;
        offset += QSL("\nUploaders: ") + m_uploaders;
        offset += QSL("\nVersion: ") + m_version;
        offset += QSL("\nHomepage: ") + m_homepage;
        offset += QSL("\nSource: ") + m_source;
        offset += QSL("\nInstalled-Size: ") + QString::number(calc_size(m_dir.path()) / 1000);
        offset += QSL("\nArchitecture: ") + architecture_name(m_arch);

        if (!m_depends.isEmpty())
                offset += QSL("\nDepends: ") + m_depends;

        offset += QSL("\nReplace: ") + m_replace;
        offset += QSL("\nSection: ") + m_section;
        offset += QSL("\nDescription: ") + m_desc_title;
        if (m_desc_body != "")
                offset += QSL("\n             ") + m_desc_body;

        return offset;
}


qint64 package::calc_size(const QString &_dir)
{
        qint64 sizex = 0;
        QFileInfo str_info(_dir);
        if (!str_info.isDir()) {
                return str_info.size();
        }

        QDir dir(_dir);
        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for (int i = 0; i < list.size(); ++i) {
                QFileInfo fileInfo = list.at(i);
#ifdef QT_DEBUG
                qDebug() << fileInfo.absoluteFilePath();
#endif
                if(fileInfo.isDir() && fileInfo.baseName() != "DEBIAN")
                        sizex += calc_size(fileInfo.absoluteFilePath());
                else if(fileInfo.isFile())
                        sizex += fileInfo.size();
        }
        return sizex;
}

QString package::architecture_name(const arch_t &arch)
{
        switch (arch) {
        case arch_t::all:
                return QSL("all");
        case arch_t::amd64:
                return QSL("amd64");
        case arch_t::i386:
                return QSL("i386");
        default:
                return "";
        }
}

arch_t package::architecture_value(const QString &arch)
{
        if(arch.compare(QSL("all"), Qt::CaseInsensitive))
                return arch_t::all;
        else if(arch.compare(QSL("amd64"), Qt::CaseInsensitive))
                return arch_t::amd64;
        else if(arch.compare(QSL("i386"), Qt::CaseInsensitive))
                return arch_t::i386;
        else
                return arch_t::error;
}
