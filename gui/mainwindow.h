#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

#include "pkgwindow.h"

#include <debcreator.h>

#include "about.h"
#include "depend.h"

namespace Ui
{
class mainwindow;
}

class mainwindow : public QMainWindow
{
        Q_OBJECT

public:
        explicit mainwindow(QWidget *parent = nullptr);
        ~mainwindow();

        package_list* m_pkglist;
        database *m_db;
private:
        Ui::mainwindow *ui;
        about* ui_about;

private slots:
        void reload();
        void open(const QString &name);
        void close(int index);

        void package_add();
        void package_remove();
        void control_load();
};

#endif // MAINWINDOW_H
