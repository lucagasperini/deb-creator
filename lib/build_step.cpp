#include "build_step.h"
#include "define.h"

build_step::build_step(QObject *parent) : QObject(parent)
{

}

build_step::build_step(int id, int pkg, QString app, QString arg, QString dir, QObject *parent) :
        QObject(parent),
        m_id(id),
        m_pkg(pkg),
        m_app(app),
        m_arg(arg),
        m_dir(dir)
{

}

build_step::build_step(const build_step &step)
{
        m_id = step.m_id;
        m_pkg = step.m_pkg;
        m_app = step.m_app;
        m_arg = step.m_arg;
        m_dir = step.m_dir;
}

QString build_step::shell()
{
        return QSL("[ ") + m_dir + QSL(" ]$ ") + m_app + QSL(" ") + m_arg;
}
