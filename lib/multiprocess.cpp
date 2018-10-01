#include "multiprocess.h"
#include "define.h"

multiprocess::multiprocess(QObject *parent) : QThread(parent)
{
        m_pro = new QList<QProcess*>;
}

void multiprocess::run()
{
        QProcess* buffer = nullptr;

        for(int i = 0; i < m_pro->size(); i++) {
                buffer = m_pro->at(i);
                if(buffer == nullptr)
                        return;
#ifdef QT_DEBUG
                qDebug() << QSL("Executing: ") << buffer->program() << buffer->arguments() << buffer->workingDirectory();
#endif
                buffer->start(QIODevice::ReadWrite);
                while(buffer->waitForReadyRead()) {
                        emit read(buffer->readAll());
                }

                buffer->waitForFinished();
        }
}

void multiprocess::append(const QString &program)
{
        QProcess* step = new QProcess;
        step->setProgram(program);
        m_pro->append(step);
}

void multiprocess::append(const QString &program, const QStringList &args)
{
        QProcess* step = new QProcess;
        step->setProgram(program);
        step->setArguments(args);
        m_pro->append(step);
}

void multiprocess::append(const QString &program, const QStringList &args, const QString &working_dir)
{
        QProcess* step = new QProcess;
        step->setProgram(program);
        step->setArguments(args);
        if(working_dir.isEmpty())
                step->setWorkingDirectory(DEB_CREATOR_TMP);
        else
                step->setWorkingDirectory(working_dir);

        m_pro->append(step);
}

void multiprocess::clear()
{
        m_pro->clear();
}

bool multiprocess::is_empty()
{
        return m_pro->isEmpty();
}
