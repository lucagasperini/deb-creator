#include "multiprocess.h"
#include "define.h"

multiprocess::multiprocess(QObject *parent) : QThread(parent)
{
        m_build = new QList<QProcess*>;
}

void multiprocess::run()
{
        QProcess* buffer = nullptr;

        for(int i = 0; i < m_build->size(); i++) {
                buffer = m_build->at(i);
                if(buffer == nullptr)
                        return;
#ifdef QT_DEBUG
                qDebug() << QSL("Executing: ") << buffer->program() << buffer->arguments() << buffer->workingDirectory();
#endif
                buffer->start(QIODevice::ReadWrite);

                buffer->waitForFinished();
                emit read(buffer->readAll());
        }
}

void multiprocess::build_append(const QString &program, const QStringList &args, const QString &working_dir)
{
        QProcess* step = new QProcess;
        step->setProgram(program);
        step->setArguments(args);
        if(working_dir.isEmpty())
                step->setWorkingDirectory(DEB_CREATOR_TMP);
        else
                step->setWorkingDirectory(working_dir);

        m_build->append(step);
}

void multiprocess::build_clear()
{
        m_build->clear();
}

bool multiprocess::build_is_empty()
{
        return m_build->isEmpty();
}
