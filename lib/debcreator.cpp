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


QByteArray debcreator::pkg_create(const QByteArray& control, const QString &outputfile)
{
        QProcess dpkg(this);
        QTextStream out;
        QString pkg_root = m_pkg->root();
        QDir debian_dir(pkg_root + QSL("/DEBIAN"));
        if(!debian_dir.exists())
                debian_dir.mkdir(pkg_root + QSL("/DEBIAN"));

        QFile control_file(pkg_root + QSL("/DEBIAN/control"));

        control_file.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&control_file);

        out << control;
        out << "\n";

        control_file.flush();
        control_file.close();

        QString cmd = QSL("dpkg -b ") + pkg_root + QSL(" ") + outputfile;

#ifdef QT_DEBUG
        qDebug() << QSL("Executing: ") << cmd;
#endif

        dpkg.start(cmd, QIODevice::ReadWrite);

        QByteArray data;

        while(dpkg.waitForReadyRead()) {
                data.append(dpkg.readAll());
        }
        return data;
}

QString debcreator::build_dir()
{
        return DEB_CREATOR_SRC + m_pkg->m_name;
}

bool debcreator::file_write(const QString &filename, const QString &data)
{
        QFile file(filename);

        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                return false;
        }
        file.write(data.toUtf8());

        file.flush();
        file.close();
        return true;
}

QByteArray debcreator::file_read(const QString &filename)
{
        QByteArray offset;
        QFile file(filename);

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return offset;
        }

        offset = file.readAll();
        file.close();
        return offset;
}
