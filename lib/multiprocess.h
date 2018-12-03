#ifndef MULTIPROCESS_H
#define MULTIPROCESS_H

#include "list_build_step.h"

#include <QThread>
#include <QProcess>
#include <QTextStream>

class multiprocess : public QThread
{
        Q_OBJECT
public:
        explicit multiprocess(QObject *parent = nullptr);
        /**
         * @brief run QThread while all QProcess aren't executed
         */
        void run();
        list_build_step *m_build;
signals:
        void read(const QByteArray &text);
};

#endif // MULTIPROCESS_H
