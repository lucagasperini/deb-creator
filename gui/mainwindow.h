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
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        debcreator *m_api;
private:
        Ui::MainWindow *ui;
        about about;

private slots:
        void output_file();
        void working_dir();
        void clear_output();
        void create_package();
        void generate_control();
};

#endif // MAINWINDOW_H
