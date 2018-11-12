#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

#include <debcreator.h>

#include "about.h"
#include "depend.h"
#include "build_editor.h"

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
        package *m_pkg;
        multiprocess *m_process;
        changelog *m_changelog;
        database *m_db;

        void load();
        void save();
private:
        Ui::mainwindow *ui;
        depend* ui_dep;
        about* ui_about;

        QFileSystemModel *m_model_custom;
        QFileSystemModel *m_model_compile;

private slots:
        void welcome_reload();
        void welcome_add();
        void welcome_remove();
        void depend_show();
        void depend_build_show();
        void output_append(const QString &text);
        void output_clear();
        void package_generate();
        void control_generate();
        void control_database(const QString &str);
        void control_load();
        void changelog_generate();
        void changelog_refresh();
        void changelog_change();
        void compile_directory();
        void compile_refresh();
        void compile();
        void build_add();
        void build_remove();
        void build_save();
        void custom_refresh();
        void custom_save();
        void custom_load(const QModelIndex &a);
};

#endif // MAINWINDOW_H
