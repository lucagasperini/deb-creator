#ifndef APT_H
#define APT_H

#include "package.h"

#include <QObject>
#include <QList>
#include <QProcess>


class apt : public QProcess
{
        Q_OBJECT
public:
        explicit  apt(QObject *parent = nullptr);
        QList<package*>* search(const QString &text);

};

#endif // APT_H
