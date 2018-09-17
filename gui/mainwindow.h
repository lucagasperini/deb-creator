#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <debcreator.h>

#include "about.h"

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
        about about;

private slots:
        void output_file();
        void working_dir();
        void compile_dir();
        void clear_output();
        void create_package();
        void generate_control();
        void generate_changelog();
        void check_database(const QString &package);
        void fetch_changelog();
        void save_project();
        void compile_refresh();
        void compile();
};

#endif // MAINWINDOW_H
