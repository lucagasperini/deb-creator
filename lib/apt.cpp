#include "apt.h"
#include "define.h"

apt::apt(QObject *parent) : QProcess(parent)
{

}

QList<package*>* apt::search(const QString &text)
{
        if(text.isEmpty())
                return nullptr;

        QList<package*>* offset = new QList<package*>;
        QString cmd = APT_SEARCH + text + QSL(" --full");

#ifdef QT_DEBUG
        qDebug() << QSL("Executing: ") << cmd;
#endif

        start(cmd, QIODevice::ReadWrite);

        waitForFinished();
        QString input = readAll();
        QStringList buffer = input.split("\n\n");
        buffer.removeLast();
        package* pkg;
        for (int i = 0; i < buffer.size(); i++) {
                pkg = new package(buffer.at(i));
                offset->append(pkg);
        }
        return offset;
}
