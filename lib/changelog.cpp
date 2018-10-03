#include "changelog.h"
#include "define.h"
#include "git.h"

#include <QDateTime>

changelog::changelog(QString file, QObject *parent) : QObject(parent), m_file(file)
{

}

bool changelog::generate(const package *pkg, const QString &text, const QString &status, const QString &urgency)
{
        QDateTime now = QDateTime::currentDateTime();
        git info;

        m_text = pkg->m_name.toUtf8() + " (" + pkg->m_version.toUtf8() + ") " + status.toUtf8() + "; urgency=" + urgency.toUtf8() + "\n\n" +
                 text.toUtf8() + "\n\n" +
                 " -- " + info.fetch_user().toUtf8() + " " + now.toString(QSL("ddd, dd MMM yyyy hh:mm:ss t")).toUtf8();
        return true;
}

QStringList changelog::fetch()
{
        if(m_file.isEmpty())
                return QStringList();

        QStringList offset;
        QString buffer;
        QFile changelog_file(m_file);

        if(!changelog_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return QStringList();
        }

        QTextStream in(&changelog_file);

        while (!in.atEnd()) {
                QString line = in.readLine();
                buffer.append(line);
                if(line.startsWith(QSL(" -- "))) {
                        offset.append(buffer);
                        buffer.clear();
                }
        }
        changelog_file.close();

        return offset;
}

void changelog::save()
{
        if(m_text.isEmpty())
                return;
        QFile changelog_file(m_file);

        changelog_file.open(QIODevice::Append | QIODevice::Text);

        changelog_file.write(m_text);
        changelog_file.write("\n");

        changelog_file.flush();
        changelog_file.close();
}
