#include "apt.h"
#include "define.h"

apt::apt(QObject *parent) : QProcess(parent)
{

}

QByteArray apt::exec()
{
        QByteArray offset;
        start(QIODevice::ReadWrite);
#ifdef QT_DEBUG
        qDebug() << QSL("Executing: ") << program() << arguments() << workingDirectory();
#endif
        waitForFinished();
        offset = readAll();
        close();
        return offset;
}

QList<package*>* apt::search(const QString &text)
{
        if(text.isEmpty())
                return nullptr;

        QList<package*>* offset = new QList<package*>;
        QStringList args;

        setProgram(QSL("apt-cache"));
        args << QSL("search") << text << QSL("--full");
        setArguments(args);

        QString input = exec();
        QStringList buffer = input.split("\n\n");
        buffer.removeLast();
        package* pkg;
        for (int i = 0; i < buffer.size(); i++) {
                pkg = new package(buffer.at(i));
                offset->append(pkg);
        }
        return offset;
}
