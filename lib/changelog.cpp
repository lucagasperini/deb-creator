#include "changelog.h"
#include "define.h"
#include "git.h"
#include "filesystem.h"

#include <QDateTime>

changelog::changelog(QObject *parent) : QObject(parent)
{

}

changelog::changelog(const QByteArray& text, QObject *parent)  : QObject(parent), m_text(text)
{

}

QByteArray changelog::generate(const package *pkg, const QString &text, const QString &status, const QString &urgency)
{
        QDateTime now = QDateTime::currentDateTime();
        git info;

        return pkg->m_name.toUtf8() + " (" + pkg->m_version.toUtf8() + ") " + status.toUtf8() + "; urgency=" + urgency.toUtf8() + "\n\n" +
               text.toUtf8() + "\n\n" +
               " -- " + info.fetch_user().toUtf8() + " " + now.toString(QSL("ddd, dd MMM yyyy hh:mm:ss t")).toUtf8();
}

QList<changelog*> *changelog::fetch(const QString &file)
{
        QByteArray buffer;
        QByteArray line;
        QFile changelog_file(file);

        if(!changelog_file.open(QIODevice::ReadOnly | QIODevice::Text))
                return nullptr;

        QList<changelog*> *cl = new QList<changelog*>;
        while (!changelog_file.atEnd()) {
                line = changelog_file.readLine();
                buffer.append(line);
                if(line.startsWith(" -- ")) {
                        cl->append(new changelog(buffer));
                        buffer.clear();
                }
        }
        changelog_file.close();
}

void changelog::save(const QString &file, const QByteArray &text)
{
        if(text.isEmpty() || file.isEmpty())
                return;
        filesystem::file_write(file, text);
}

void changelog::clear()
{
        m_text.clear();
}

QString changelog::title()
{
        return m_text.split('\n').at(0);
}
