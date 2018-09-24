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
         * @brief compile source code in @public m_build_dir and using @public m_build processes
         * @return Processes output stream
         */
        void run();
        /**
         * @brief build_append add a step build in @public m_build
         * @param program path of the program
         * @param args arguments of the program
         * @param working_dir directory where source code is
         */
        void build_append(const QString &program, const QStringList &args, const QString &working_dir = "");
        /**
         * @brief build_clear erase all step build in @public m_build
         */
        void build_clear();
        /**
         * @brief build_is_empty check if @public m_build is empty
         * @return if there are step build it will return false, otherwise true
         */
        bool build_is_empty();
signals:
        void read(const QByteArray &text);
private:
        QList<QProcess*> *m_build;
};

#endif // MULTIPROCESS_H
