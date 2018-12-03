#include "multiprocess.h"
#include "define.h"

multiprocess::multiprocess(QObject *parent) : QThread(parent)
{
        m_build = new list_build_step;
}

void multiprocess::run()
{
        QProcess buffer;
        build_step* p;
        for(int i = 0; i < m_build->size(); i++) {
                p = m_build->at(i);
                buffer.setProgram(p->m_app);
                buffer.setArguments(p->m_arg.split(' '));
                buffer.setWorkingDirectory(p->m_dir);
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
