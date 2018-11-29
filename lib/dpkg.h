#ifndef DPKG_H
#define DPKG_H

#include <QProcess>
#include <changelog.h>

class dpkg : public QProcess
{
        Q_OBJECT
public:
        explicit dpkg(const QString &pkg_root, QObject *parent = nullptr);
        /**
         * @brief package create the package using QProcess to dpkg
         * @param control is the control text
         * @param outputfile the path where package is created
         */
        void control(const QByteArray &control);
        void changelogs(const QList<changelog*> *cl);
        QByteArray exec(const QString &outputfile);

};

#endif // DPKG_H
