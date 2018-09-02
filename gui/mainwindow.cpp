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

        connect(ui->btn_gencontrol, &QPushButton::clicked, this, &MainWindow::generate_control);
        connect(ui->btn_createpackage, &QPushButton::clicked, this, &MainWindow::create_package);
        connect(ui->btn_clear, &QPushButton::clicked, this, &MainWindow::clear_output);
        connect(ui->btn_filesystem, &QPushButton::clicked, this, &MainWindow::working_dir);
        connect(ui->btn_output_file, &QPushButton::clicked, this, &MainWindow::output_file);
        connect(ui->a_create_package, &QAction::triggered, this, &MainWindow::create_package);
        connect(ui->a_generate_control, &QAction::triggered, this, &MainWindow::generate_control);
        connect(ui->a_quit, &QAction::triggered, qApp, &QApplication::quit);
        connect(ui->a_about, &QAction::triggered, this, &MainWindow::about);
        connect(ui->a_aboutqt, &QAction::triggered, qApp, &QApplication::aboutQt);
        connect(ui->ck_dependency, &QCheckBox::toggled, ui->ln_dependancies, &QLineEdit::setEnabled);
        // connect(ui->a_manual) TODO: Add a manual?

        m_api = new debcreator;
}

MainWindow::~MainWindow()
{
        delete ui;
}

void MainWindow::generate_control()
{
        m_api->m_package = ui->ln_projectname->text();
        m_api->m_version = ui->ln_version->text();
        m_api->m_arch = ui->ln_architecture->text();
        m_api->m_depends = ui->ln_dependancies->text();
        m_api->m_maintainer = ui->ln_maintainer->text();
        m_api->m_desc_title = ui->ln_descriptiontitle->text();
        m_api->m_desc_body = ui->ln_description->text();
        m_api->m_dir = ui->ln_filesystem->text();
        m_api->m_outputfile = ui->ln_outputfile->text();

        ui->txt_control->setText(m_api->control());
}

void MainWindow::create_package()
{
        QString output = m_api->package(ui->txt_control->toPlainText());
        ui->txt_output->setText(output);
}

void MainWindow::clear_output()
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

void MainWindow::working_dir()
{
        QFileDialog dialog(this);
        QString dir;
        dialog.setViewMode(QFileDialog::Detail);
        dir = dialog.getExistingDirectory();
        ui->ln_filesystem->setText(dir);
}

void MainWindow::output_file()
{
        QFileDialog dialog(this);
        QString file;
        dialog.setViewMode(QFileDialog::Detail);
        file = dialog.getSaveFileName();
        ui->ln_outputfile->setText(file);
}

void MainWindow::about()
{
        QMessageBox about;
        about.about(this, "About", " About Deb-Creator\n\nDeb-Creator is a simple application designed to make the creation of debian packages easier. It aims to provide a straight forward graphical user interface and to automaticaly generate control files and debian packages without the need for any command line knowledge.\n\nBen Heidemann\nLuca Gasperini");
}
