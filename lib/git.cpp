#include "git.h"
#include "define.h"

git::git(QObject *parent) : QObject(parent), m_git(new QProcess)
{
        m_git->setProgram(QSL("git"));
        m_git->setWorkingDirectory(DEB_CREATOR_SRC);
}

QByteArray git::execute()
{
        m_git->start(QIODevice::ReadOnly);
#ifdef QT_DEBUG
        qDebug() << QSL("Executing: ") << m_git->program() << m_git->arguments() << m_git->workingDirectory();
#endif
        m_git->waitForFinished();
        QByteArray offset = m_git->readAll().trimmed();
        m_git->close();
        return offset;
}

QString git::clone(const QUrl &url, const QString &directory)
{
        QStringList args;
        QString offset;
        if(directory.isEmpty()) {
                args << QSL("clone") << url.toString();
                offset = url.fileName().remove(url.fileName().right(4));
        } else {
                args << QSL("clone") << url.toString() << directory;
                offset = directory; //FIXME: Maybe directory is relative path
        }
        m_git->setArguments(args);
        execute();
        return offset;
}

QString git::fetch_user()
{
        QString name;
        QString mail;
        QStringList args;

        args << QSL("config") << QSL("--get") << QSL("user.name");
        m_git->setArguments(args);
        name = execute();
        args.clear();

        args << QSL("config") << QSL("--get") << QSL("user.email");
        m_git->setArguments(args);
        mail = execute();

        return name + " " + "<" + mail + ">";
}
