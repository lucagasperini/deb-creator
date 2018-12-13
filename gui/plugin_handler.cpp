#include "plugin_handler.h"

plugin_handler::plugin_handler(QObject *parent) :
        QObject (parent),
        m_plugin(new QList<plugin*>)
{

}
