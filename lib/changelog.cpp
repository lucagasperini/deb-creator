#include "changelog.h"
#include "define.h"
#include "git.h"

#include <QDateTime>

changelog::changelog(const package *pkg, QObject *parent) : QObject(parent), m_pkg(pkg)
{

}

QByteArray changelog::generate(const QString &text, const QString &status, const QString &urgency)
{
        QDateTime now = QDateTime::currentDateTime();
        git info;

        return m_pkg->m_name.toUtf8() + " (" + m_pkg->m_version.toUtf8() + ") " + status.toUtf8() + "; urgency=" + urgency.toUtf8() + "\n\n" +
               text.toUtf8() + "\n\n" +
               " -- " + info.fetch_user().toUtf8() + " " + now.toString(QSL("ddd, dd MMM yyyy hh:mm:ss t")).toUtf8();
}

void changelog::fetch()
{
        if(m_pkg->m_dir.isEmpty())
                return;

        clear();
        QByteArray buffer;
        QByteArray line;
        QFile changelog_file(m_pkg->m_dir.path() + QSL("/DEBIAN/changelog"));
        changelog_t *cl;
        int i = 0;

        if(!changelog_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return;
        }

        while (!changelog_file.atEnd()) {
                line = changelog_file.readLine();
                buffer.append(line);
                if(line.startsWith(" -- ")) {
                        cl = new changelog_t;
                        cl->index = i++;
                        cl->text = buffer;
                        m_cl.append(cl);
                        buffer.clear();
                }
        }
        changelog_file.close();

        return;
}

void changelog::save(const QByteArray &text)
{
        if(text.isEmpty())
                return;
        QFile changelog_file(m_pkg->m_dir.path() + QSL("/DEBIAN/changelog"));

        changelog_file.open(QIODevice::Append | QIODevice::Text);

        changelog_file.write(text);
        changelog_file.write("\n");

        changelog_file.flush();
        changelog_file.close();
}

void changelog::clear()
{
        m_cl.clear();
}

QStringList changelog::titles()
{
        QStringList offset;
        for(int i = 0; i < m_cl.size(); i++)
                offset << m_cl.at(i)->text.split('\n').at(0);
        return offset;
}

QByteArray changelog::text(int i)
{
        return m_cl.at(i)->text;
}
