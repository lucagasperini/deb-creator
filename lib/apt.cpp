#include "apt.h"
#include "define.h"

apt::apt(QObject *parent) : QProcess(parent)
{

}

QList<package*>* apt::search(const QString &text)
{
        QList<package*>* offset = new QList<package*>();
        QString cmd = APT_SEARCH + text;

#ifdef QT_DEBUG
        //qDebug() << QSL("Executing: ") << cmd;
#endif

        start(cmd, QIODevice::ReadWrite);

        waitForFinished();
        QString line;
        QStringList buffer;
        package* pkg;
        while (!atEnd()) {
                line = readLine();
                buffer = line.split(QSL(" - "));
                pkg = new package;
                pkg->m_name = buffer.at(0);
                pkg->m_desc = buffer.at(1);
                offset->append(pkg);
        }
        return offset;
}
