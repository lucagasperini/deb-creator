#include "plugin.h"

#include <QPluginLoader>

plugin::plugin(const QString path, QObject *parent) :
    QObject(parent),
    m_interface(nullptr),
    m_path(path),
    m_active(false)
{

}

int plugin::load()
{
    QPluginLoader pluginLoader(m_path);
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        m_interface = qobject_cast<plugin_interface*>(plugin);
        m_interface->generate();
    }
    return 0;
}
