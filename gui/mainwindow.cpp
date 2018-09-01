#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    ui->ln_dependancies->setEnabled(checked);
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox msg;
    QString package;
    QString version;
    QString arch;
    QString depends;
    QString maintainer;
    QString desc_title;
    QString desc_body;
    QString desc;
    string pkg_str;

    package = ui->ln_projectname->text();
    pkg_str = package.toStdString();

    //qDebug() << pkg_str.find(" ") << " : " << pkg_str.length();
    if (pkg_str.find(" ") < pkg_str.length())
    {
        msg.setText("Package name must not contain spaces.");
        msg.exec();
        return;
    }

    version = ui->ln_version->text();
    arch = ui->ln_architecture->text();
    if (arch == "")
    {
        arch = "all";
    }
    if (ui->checkBox->isChecked() == true && ui->ln_dependancies->text() != "")
    {
        depends = "\ndepends: " + ui->ln_dependancies->text();
    }
    else
    {
        depends = "";
    }
    maintainer = ui->ln_maintainer->text();
    desc_title = ui->ln_descriptiontitle->text();
    desc_body = ui->ln_description->text();
    desc = "\ndescription: "+desc_title;
    if (desc_body != "")
    {
        desc += "\n             "+desc_body;
    }

    ui->txt_control->setText("package: "+package+"\nversion: "+version+"\narchitecture: "+arch+depends+"\nmaintainer: "+maintainer+desc+"\n");
}

void MainWindow::on_btn_createpackage_clicked()
{
    QProcess dpkg(this);
    QDir debian_dir(ui->ln_filesystem->text()+"/DEBIAN/");
    debian_dir.mkdir(ui->ln_filesystem->text()+"/DEBIAN/");
    QFile control_file(ui->ln_filesystem->text()+"/DEBIAN/control");

    control_file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&control_file);

    out << ui->txt_control->toPlainText();
    out << "\n";

    control_file.flush();
    control_file.close();

    qDebug() << "dpkg -b "+ui->ln_filesystem->text()+" "+ui->ln_outputdir->text();
    dpkg.start("dpkg -b "+ui->ln_filesystem->text()+" "+ui->ln_outputdir->text(), QIODevice::ReadWrite);

    QByteArray data;

    while(dpkg.waitForReadyRead())
    {
        data.append(dpkg.readAll());
    }

    ui->txt_output->setText(data.data());
}

void MainWindow::on_btn_clear_clicked()
{
    /*QMessageBox msg_clear;
    QPushButton all;
    QPushButton output;

    all.setText("Clear all");
    output.setText("Only output");

    //msg_clear.addButton();
    msg_clear.addButton("Only output", QMessageBox::ButtonRole );*/
    ui->txt_output->setText("");
}

void MainWindow::on_btn_filesystem_clicked()
{
    QFileDialog dialog(this);
    QString dirName;
    dialog.setViewMode(QFileDialog::Detail);
    dirName = dialog.getExistingDirectory();
    ui->ln_filesystem->setText(dirName);
}

void MainWindow::on_btn_outbutdir_clicked()
{
    QFileDialog dialog(this);
    QString fileName;
    dialog.setViewMode(QFileDialog::Detail);
    fileName = dialog.getSaveFileName();
    ui->ln_outputdir->setText(fileName);
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionGenerate_control_file_triggered()
{
    MainWindow::on_pushButton_clicked();
}

void MainWindow::on_actionCreate_package_triggered()
{
    MainWindow::on_btn_createpackage_clicked();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox about;
    about.about(this, "About", " About Deb-Creator\n\nDeb-Creator is a simple application designed to make the creation of debian packages easier. It aims to provide a straight forward graphical user interface and to automaticaly generate control files and debian packages without the need for any command line knowledge.\n\nBen Heidemann");
}

void MainWindow::on_btn_add_clicked()
{
    ui->txt_control->setText(ui->txt_control->toPlainText()+ui->cmb_condition->currentText()+": "+ui->ln_value->text()+"\n");
}
