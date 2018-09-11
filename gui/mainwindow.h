#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <debcreator.h>
#include <changelog.h>
#include <control.h>

#include "about.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        debcreator *m_api;
        changelog *m_changelog;
        control *m_control;
private:
        Ui::MainWindow *ui;
        about about;

private slots:
        void save_project();
        void output_file();
        void working_dir();
        void clear_output();
        void create_package();
        void generate_control();
        void generate_changelog();
        void check_database();
        void fetch_changelog(int i);
};

#endif // MAINWINDOW_H
