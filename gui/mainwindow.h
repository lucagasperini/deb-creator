#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <debcreator.h>

#include "about.h"
#include "depend.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        debcreator *m_api;
private:
        Ui::MainWindow *ui;
        depend* ui_dep;
        about* ui_about;

private slots:
        void welcome_reload();
        void welcome_add();
        void welcome_remove();
        void depend_show();
        void dir_compile();
        void append_output(const QString &text);
        void clear_output();
        void create_package();
        void generate_control();
        void changelog_generate();
        void changelog_refresh();
        void changelog_change();
        void check_database(const QString &package);
        void compile_refresh();
        void compile();
        void build_add();
        void build_remove();
        void build_save();
};

#endif // MAINWINDOW_H
