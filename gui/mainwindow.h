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

        QMap<int,QString>* m_pkglist;
        database *m_db;
private:
        Ui::mainwindow *ui;
        about* ui_about;

private slots:
        void reload();
        void open(const QString &pkg);
        void close(int index);
        void control_load();
};

#endif // MAINWINDOW_H
