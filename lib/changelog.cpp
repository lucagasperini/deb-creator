#include "changelog.h"
#include "define.h"
#include "git.h"
#include "filesystem.h"

#include <QDateTime>

changelog::changelog(QObject *parent) : QObject(parent)
{

}

changelog::changelog(int id, const package *pkg, const QString &version, const QByteArray& text, const QString &status, const QString &urgency, QObject *parent)  :
        QObject(parent),
        m_id(id),
        m_pkg(pkg),
        m_version(version),
        m_text(text),
        m_status(status),
        m_urgency(urgency)
{

}

QByteArray changelog::generate()
{
        QDateTime now = QDateTime::currentDateTime();
        git info;

        return m_pkg->m_name.toUtf8() + " (" + m_version.toUtf8() + ") " + m_status.toUtf8() + "; urgency=" + m_urgency.toUtf8() + "\n\n" +
               m_text + "\n\n" +
               " -- " + info.fetch_user().toUtf8() + " " + now.toString(QSL("ddd, dd MMM yyyy hh:mm:ss t")).toUtf8() + "\n\n";
}

void changelog::clear()
{
        m_text.clear();
}

QString changelog::title()
{
        return m_text.split('\n').at(0);
}
