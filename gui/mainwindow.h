#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_btn_add_clicked();
    void on_actionAbout_triggered();
    void on_actionCreate_package_triggered();
    void on_actionGenerate_control_file_triggered();
    void on_actionQuit_triggered();
    void on_btn_outbutdir_clicked();
    void on_btn_filesystem_clicked();
    void on_btn_clear_clicked();
    void on_btn_createpackage_clicked();
    void on_pushButton_clicked();
    void on_checkBox_toggled(bool checked);
};

#endif // MAINWINDOW_H
