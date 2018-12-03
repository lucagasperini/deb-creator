#include "dpkg.h"
#include "define.h"
#include "filesystem.h"

dpkg::dpkg(const QString &pkg_root, QObject *parent) : QProcess(parent)
{
        setProgram(QSL("dpkg"));
        setWorkingDirectory(pkg_root);
}

void dpkg::control(const QByteArray& control)
{
        QProcess dpkg(this);
        QString d_dir = workingDirectory() + QSL("/DEBIAN");
        QDir debian_dir(d_dir);
        if(!debian_dir.exists())
                debian_dir.mkdir(d_dir);

        filesystem::file_write(d_dir + QSL("/control"), control);
}

void dpkg::changelogs(const changelog_list *cl)
{
        if(cl == nullptr || cl->isEmpty())
                return;

        filesystem::file_write(workingDirectory() + QSL("/DEBIAN/changelog"), cl->text());
}

QByteArray dpkg::exec(const QString &outputfile)
{
        QString cmd = QSL("dpkg -b ") + workingDirectory() + QSL(" ") + outputfile;

#ifdef QT_DEBUG
        qDebug() << QSL("Executing: ") << cmd;
#endif

        start(cmd, QIODevice::ReadWrite);

        QByteArray data;

        while(waitForReadyRead()) {
                data.append(readAll()); //signals?
        }
        return data;
}
