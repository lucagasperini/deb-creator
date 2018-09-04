#include "debcreator.h"

#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>

#ifdef QT_DEBUG
#include <QDebug>
#endif

debcreator::debcreator(QObject *parent) : QObject(parent)
{

}

QString debcreator::control()
{
        QString offset;

        if (m_package.contains(' ')) {
                QMessageBox::warning(nullptr, "Control file error", "Package name must not contain spaces.");
                return "";
        }

        offset += ("Package: " + m_package);
        offset += ("\nMaintainer: " + m_maintainer);
        offset += ("\nUploaders: " + m_uploaders);
        offset += ("\nVersion: " + m_version);
        offset += ("\nHomepage: " + m_homepage);
        offset += ("\nSource: " + m_source);

        if (m_arch.isEmpty())
                offset += "\nArchitecture: all";
        else
                offset += ("\nArchitecture: " + m_arch);

        if (!m_depends.isEmpty())
                offset += ("\nDepends: " + m_depends);

        offset += ("\nReplace: " + m_replace);
        offset += ("\nSection: " + m_section);
        offset += ("\nDescription: " + m_desc_title);
        if (m_desc_body != "")
                offset += "\n             " + m_desc_body;

        return offset;
}

QString debcreator::package(const QString& control)
{
        QProcess dpkg(this);
        QDir debian_dir(m_dir + "/DEBIAN/");
        debian_dir.mkdir(m_dir + "/DEBIAN/");
        QFile control_file(m_dir + "/DEBIAN/control");

        control_file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&control_file);

        out << control;
        out << "\n";

        control_file.flush();
        control_file.close();

        QString cmd = "dpkg -b " + m_dir + " " + m_outputfile;

#ifdef QT_DEBUG
        qDebug() << "Executing: " << cmd;
#endif

        dpkg.start(cmd, QIODevice::ReadWrite);

        QByteArray data;

        while(dpkg.waitForReadyRead()) {
                data.append(dpkg.readAll());
        }
#ifdef QT_DEBUG
        qDebug() << data.data();
#endif
        return data.data();
}
