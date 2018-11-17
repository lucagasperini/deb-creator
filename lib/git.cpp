#include "git.h"
#include "define.h"

git::git(QObject *parent) : QProcess(parent)
{
        setProgram(QSL("git"));
        setWorkingDirectory(DEB_CREATOR_SRC);
}

QByteArray git::exec()
{
        start(QIODevice::ReadOnly);
#ifdef QT_DEBUG
        qDebug() << QSL("Executing:") << program() << arguments() << workingDirectory();
#endif
        waitForFinished();
        QByteArray offset = readAll();
        close();
        return offset;
}

QString git::clone(const QUrl &url, const QString &directory, const QString &branch)
{
        QStringList args;
        QString offset;

        if(directory.isEmpty()) {
                args << QSL("clone") << url.toString();
                if(url.fileName().endsWith(".git"))
                        offset = url.fileName().remove(url.fileName().right(4));
                else
                        offset = url.fileName();
        } else {
                args << QSL("clone") << url.toString() << directory;
                offset = directory; //FIXME: Maybe directory is relative path
        }

        if(!branch.isEmpty())
                args << QSL("-b") << branch;

        setArguments(args);
        exec();
        return offset;
}

QString git::fetch_user()
{
        QString name;
        QString mail;
        QStringList args;

        args << QSL("config") << QSL("--get") << QSL("user.name");
        setArguments(args);
        name = exec().trimmed();
        args.clear();

        args << QSL("config") << QSL("--get") << QSL("user.email");
        setArguments(args);
        mail = exec().trimmed();

        return name + " " + "<" + mail + ">";
}
