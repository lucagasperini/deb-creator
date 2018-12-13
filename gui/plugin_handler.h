#ifndef PLUGIN_HANDLER_H
#define PLUGIN_HANDLER_H

#include "plugin.h"

#include <QObject>

class plugin_handler : public QObject
{
    Q_OBJECT
public:
    explicit plugin_handler(QObject *parent = nullptr);
private:
    QList<plugin*>* m_plugin;
};

#endif // PLUGIN_HANDLER_H
