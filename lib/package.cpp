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

package::package(const QString &control)
{
        QString buffer = control;
        buffer.replace(": ", "\n");
        buffer.replace(",\n", ",");

        QStringList values = buffer.split('\n');
        for(int i = 0; i < values.size(); i+=2) {
                if(values.at(i) == QSL("Package"))
                        m_name = values.at(i+1);
                else if(values.at(i) == QSL("Maintainer"))
                        m_maintainer = values.at(i+1);
                else if(values.at(i) == QSL("Uploaders"))
                        m_uploaders = values.at(i+1);
                else if(values.at(i) == QSL("Version"))
                        m_version = values.at(i+1);
                else if(values.at(i) == QSL("Homepage"))
                        m_homepage = values.at(i+1);
                else if(values.at(i) == QSL("Source"))
                        m_source = values.at(i+1);
                else if(values.at(i) == QSL("Installed-Size"))
                        m_size = values.at(i+1).toInt();
                else if(values.at(i) == QSL("Architecture"))
                        m_arch = package::architecture_value(values.at(i+1));
                else if(values.at(i) == QSL("Depends"))
                        m_depends = values.at(i+1);
                else if(values.at(i) == QSL("Replace"))
                        m_replace = values.at(i+1);
                else if(values.at(i) == QSL("Section"))
                        m_section = values.at(i+1);
                else if(values.at(i) == QSL("Description-en")) {
                        m_desc_title = values.at(i+1);
                        int k = i + 1;
                        while(values.at(++k).startsWith(' '))
                                m_desc_body.append(values.at(k).trimmed());
                        i = k;
                }
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
        offset += QSL("\nInstalled-Size: ") + QString::number(calc_size(root()) / 1000);
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

QString package::outputfile() const
{
        return m_name + "_" + m_version + ".deb";
}

QString package::root() const
{
        QDir dir = DEB_CREATOR_PKG + m_name;
        if(!dir.exists())
                dir.mkdir(DEB_CREATOR_PKG + m_name);
        return dir.path();
}

QString package::format(const QString &str)
{
        QString offset = str;
        offset.replace(PKG_NAME, m_name);
        offset.replace(PKG_MAINTAINER, m_maintainer);
        offset.replace(PKG_UPLOADER, m_uploaders);
        offset.replace(PKG_VERSION, m_version);
        offset.replace(PKG_HOMEPAGE, m_homepage);
        offset.replace(PKG_SOURCE, m_source);
        offset.replace(PKG_SIZE, QString::number(m_size));
        offset.replace(PKG_ARCH, architecture_name(m_arch));
        offset.replace(PKG_DEPEND, m_depends);
        offset.replace(PKG_REPLACE, m_replace);
        offset.replace(PKG_SECTION, m_section);
        offset.replace(PKG_DESC_TITLE, m_desc_title);
        offset.replace(PKG_DESC_BODY, m_desc_body);
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
