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
        /**
         * @brief append add a process in bottom list
         * @param step is the build_step to append
         */
        void append(const build_step &step);
        /**
         * @brief clear erase process into the list
         */
        void clear();
        /**
         * @brief is_empty check if the process list is empty
         * @return if there are process it will return false, otherwise true
         */
        bool is_empty();
signals:
        void read(const QByteArray &text);
private:
        QList<build_step*> *m_pro;
};

#endif // MULTIPROCESS_H
