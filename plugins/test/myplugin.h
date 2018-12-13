#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include <gui/plugin_interface.h>

class MyPlugin : public QObject, public plugin_interface
{
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.xsoftware.creator.plugins.myplugin")
        Q_INTERFACES(plugin_interface)
public:
        explicit MyPlugin();
        void generate() override;
        void init(form_handler* p) override;
};

#endif //MYPLUGIN_H
