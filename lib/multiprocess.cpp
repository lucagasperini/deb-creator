#include "multiprocess.h"
#include "define.h"

multiprocess::multiprocess(QObject *parent) : QThread(parent)
{
        m_pro = new QList<build_step*>;
}

void multiprocess::run()
{
        QProcess buffer;
        for(int i = 0; i < m_pro->size(); i++) {
                buffer.setProgram(m_pro->at(i)->m_app);
                buffer.setArguments(m_pro->at(i)->m_arg.split(' '));
                buffer.setWorkingDirectory(m_pro->at(i)->m_dir);
#ifdef QT_DEBUG
                qDebug() << QSL("Executing:") << buffer.program() << buffer.arguments() << buffer.workingDirectory();
#endif
                buffer.start(QIODevice::ReadWrite);
                while(buffer.waitForReadyRead()) {
                        emit read(buffer.readAll());
                }

                buffer.waitForFinished();
        }
}

void multiprocess::append(const build_step &step)
{
        m_pro->append(new build_step(step));
}

void multiprocess::clear()
{
        m_pro->clear();
}

bool multiprocess::is_empty()
{
        return m_pro->isEmpty();
}
