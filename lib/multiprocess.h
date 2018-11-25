#ifndef MULTIPROCESS_H
#define MULTIPROCESS_H

#include <QThread>
#include <QProcess>
#include <QTextStream>
#include <build_step.h>

class multiprocess : public QThread
{
        Q_OBJECT
public:
        explicit multiprocess(QObject *parent = nullptr);
        /**
         * @brief run QThread while all QProcess aren't executed
         */
        void run();
        QList<build_step*> *m_build;
signals:
        void read(const QByteArray &text);
};

#endif // MULTIPROCESS_H
