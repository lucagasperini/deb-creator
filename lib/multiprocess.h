#ifndef MULTIPROCESS_H
#define MULTIPROCESS_H

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
        /**
         * @brief append add a process in bottom list
         * @param program path of the program
         */
        void append(const QString &program);
        /**
         * @brief append add a process in bottom list
         * @param program path of the program
         * @param args arguments of the program
         */
        void append(const QString &program, const QStringList &args);
        /**
         * @brief append add a process in bottom list
         * @param program path of the program
         * @param args arguments of the program
         * @param working_dir directory where execute the process
         */
        void append(const QString &program, const QStringList &args, const QString &working_dir);
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
        QList<QProcess*> *m_pro;
};

#endif // MULTIPROCESS_H
