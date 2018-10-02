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
        void compile_dir();
        void append_output(const QString &text);
        void clear_output();
        void create_package();
        void generate_control();
        void generate_changelog();
        void check_database(const QString &package);
        void fetch_changelog();
        void compile_refresh();
        void compile();
        void build_add();
        void build_remove();
        void build_save();
};

#endif // MAINWINDOW_H
