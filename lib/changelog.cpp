#include "changelog.h"
#include <QFile>
#include <QDir>
#include <QTextStream>

changelog::changelog(QObject *parent) : QObject(parent)
{

}

QString changelog::generate(const QString &text, const QString &status, const QString &urgency)
{
        return (debcreator::m_package + " (" + debcreator::m_version + ") " + status + "; urgency=" + urgency + "\n\n" +
                       text + "\n\n" +
                       " -- " + debcreator::git_fetch_user() + " " + debcreator::date_fetch());
}

QStringList changelog::fetch(const QString &file)
{
        if(file.isEmpty())
                return QStringList();

        QStringList offset;
        QString buffer;
        QFile changelog_file(file);

        if(!changelog_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                return QStringList();
        }

        QTextStream in(&changelog_file);

        while (!in.atEnd()) {
                QString line = in.readLine();
                buffer.append(line);
                if(line.startsWith(" -- ")) {
                        offset.append(buffer);
                        buffer.clear();
                }
        }
        changelog_file.close();

        return offset;
}

QString changelog::save()
{
        QTextStream out;
        QDir debian_dir(debcreator::m_dir + "/DEBIAN/");
        if(!debian_dir.exists())
                debian_dir.mkdir(debcreator::m_dir + "/DEBIAN/");


        if(!m_text.isEmpty()) {
        QFile changelog_file(debcreator::m_dir + "/DEBIAN/changelog");

        changelog_file.open(QIODevice::Append | QIODevice::Text);
        out.setDevice(&changelog_file);

        out << m_text;
        out << "\n";

        changelog_file.flush();
        changelog_file.close();
        }
}
