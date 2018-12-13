#ifndef DPKG_H
#define DPKG_H

#include <QProcess>
#include "changelog_list.h"

class dpkg : public QProcess
{
        Q_OBJECT
public:
        explicit dpkg(const QString &pkg_root, QObject *parent = nullptr);
        /**
         * @brief control write in control file @param control
         * @param control ByteArray of control text
         */
        void control(const QByteArray &control);
        /**
         * @brief changelogs write in changelog file @param cl text
         * @param cl pointer of changelog list
         */
        void changelogs(const changelog_list *cl);
        /**
         * @brief exec create the package using QProcess to dpkg
         * @param outputfile the path where package is created
         */
        QByteArray exec(const QString &outputfile);

};

#endif // DPKG_H
