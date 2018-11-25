#include "multiprocess.h"
#include "define.h"

multiprocess::multiprocess(QObject *parent) : QThread(parent)
{
        m_build = new QList<build_step*>;
}

void multiprocess::run()
{
        QProcess buffer;
        for(int i = 0; i < m_build->size(); i++) {
                buffer.setProgram(m_build->at(i)->m_app);
                buffer.setArguments(m_build->at(i)->m_arg.split(' '));
                buffer.setWorkingDirectory(m_build->at(i)->m_dir);
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
