#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include "plugin_interface.h"

class plugin : public QObject
{
    Q_OBJECT
public:
    explicit plugin(const QString path, QObject *parent = nullptr);
    int load();
private:
    plugin_interface* m_interface;
    QString m_path;
    bool m_active;
};

#endif // PLUGIN_H
