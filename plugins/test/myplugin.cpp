#include "myplugin.h"
#include <QMessageBox>

MyPlugin::MyPlugin() : QObject (nullptr)
{

}

void MyPlugin::generate()
{
    QMessageBox::warning(nullptr, "plugin", "CIAO SONO UN PLUGIN!");
}

void MyPlugin::init(form_handler* p)
{

}
