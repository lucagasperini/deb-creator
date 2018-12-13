#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <QtPlugin>

class form_handler;


class plugin_interface
{
public:
        virtual ~plugin_interface() = 0;
        virtual void generate() = 0;
        virtual void init(form_handler* p) = 0;

};

Q_DECLARE_INTERFACE(plugin_interface, "org.xsoftware.creator.plugin")

#endif // PLUGIN_INTERFACE_H
