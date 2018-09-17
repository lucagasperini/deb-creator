#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>

#ifdef QT_DEBUG
#include <QDebug>
#endif

#define DEB_CREATOR_LOCAL       QDir::homePath() + QSL("/.local/share/deb-creator/")
#define DEB_CREATOR_DB          DEB_CREATOR_LOCAL + QSL("deb-creator.db")

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
        ui->setupUi(this);

        ui->tab_control->setEnabled(false);
        ui->tab_changelog->setEnabled(false);
        ui->tabWidget->setCurrentIndex(0);

        connect(ui->btn_save, &QPushButton::clicked, this, &MainWindow::save_project);
        connect(ui->btn_gencontrol, &QPushButton::clicked, this, &MainWindow::generate_control);
        connect(ui->btn_createpackage, &QPushButton::clicked, this, &MainWindow::create_package);
        connect(ui->btn_clear, &QPushButton::clicked, this, &MainWindow::clear_output);
        connect(ui->btn_filesystem, &QPushButton::clicked, this, &MainWindow::working_dir);
        connect(ui->btn_output_file, &QPushButton::clicked, this, &MainWindow::output_file);
        connect(ui->btn_changelog, &QPushButton::clicked, this, &MainWindow::generate_changelog);

        connect(ui->a_create_package, &QAction::triggered, this, &MainWindow::create_package);
        connect(ui->a_generate_control, &QAction::triggered, this, &MainWindow::generate_control);
        connect(ui->a_quit, &QAction::triggered, qApp, &QApplication::quit);
        connect(ui->a_about, &QAction::triggered, &about, &about::show);
        connect(ui->a_aboutqt, &QAction::triggered, qApp, &QApplication::aboutQt);
        connect(ui->ck_dependency, &QCheckBox::toggled, ui->ln_dependancies, &QLineEdit::setEnabled);
        connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::fetch_changelog);
        connect(ui->lsw_welcome, &QListWidget::currentTextChanged, this, &MainWindow::check_database);
        // connect(ui->a_manual) TODO: Add a manual?

        QDir local(DEB_CREATOR_LOCAL);
        if(!local.exists())
                local.mkdir(DEB_CREATOR_LOCAL);

        m_api = new debcreator(DEB_CREATOR_DB);

        QStringList list = m_api->db_fetch();

        if(list.isEmpty()) {
                ui->lsw_welcome->hide();
                return;
        }

        ui->lsw_welcome->show();
        ui->lsw_welcome->addItems(list);

}

MainWindow::~MainWindow()
{
        delete ui;
}

void MainWindow::generate_control()
{
        if(ui->ln_projectname->text().isEmpty()) {
                QMessageBox::critical(this, QSL("Control file error"), QSL("Package name value is empty!\nPlease add a valid package name."));
                return;
        }
        if (ui->ln_projectname->text().contains(' ')) {
                QMessageBox::warning(this, QSL("Control file error"), QSL("Package name must not contain spaces."));
                return;
        }

        ui->txt_output->append(QSL("Generating new control file..."));

        m_api->m_package = ui->ln_projectname->text();
        m_api->m_version = ui->ln_version->text();
        m_api->m_arch = ui->cb_arch->currentText();
        m_api->m_depends = ui->ln_dependancies->text();
        m_api->m_maintainer = ui->ln_maintainer->text();
        m_api->m_desc_title = ui->ln_descriptiontitle->text();
        m_api->m_desc_body = ui->txt_description->toPlainText();
        m_api->m_homepage = ui->ln_homepage->text();
        m_api->m_replace = ui->ln_replace->text();
        m_api->m_section = ui->ln_section->text();
        m_api->m_source = ui->ln_source->text();
        m_api->m_uploaders = ui->ln_uploaders->text();

        if(m_api->db_insert())
                ui->txt_output->append(QSL("Added package into database..."));
        else
                ui->txt_output->append(QSL("Failed while adding the package to the database!"));

        if(ui->ln_outputfile->text().isEmpty())
                ui->ln_outputfile->setText(QDir::homePath() + "/" + m_api->m_package + "_" + m_api->m_version + ".deb");

        ui->txt_control->setText(m_api->control());
}

void MainWindow::generate_changelog()
{
        m_api->changelog(ui->txt_changelog->toPlainText(), ui->ln_status->text(), ui->cb_urgency->currentText());
        ui->txt_changelog->setText(m_api->m_changelog);
}

void MainWindow::create_package()
{
        m_api->m_dir = ui->ln_filesystem->text();
        m_api->m_outputfile = ui->ln_outputfile->text();

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
        QDir dir;
        dir = QFileDialog::getExistingDirectory(this, QSL("Source file of the package"));
        ui->ln_filesystem->setText(dir.absolutePath());

        if(!dir.isEmpty() && ui->ln_outputfile->text().isEmpty()) {
                QString name;
                name = "/" + dir.dirName() + ".deb";
                dir.cd("..");
                ui->ln_outputfile->setText(dir.absolutePath() + name);
        }
}

void MainWindow::output_file()
{
        QString file;
        file = QFileDialog::getSaveFileName(this, QSL("Select where save package"), ui->ln_outputfile->text());
        ui->ln_outputfile->setText(file);
}


void MainWindow::check_database(const QString &package)
{
        if(!m_api->db_fetch(package)) {
                ui->txt_output->append(ui->ln_projectname->text() + QSL(" package didn't find!"));
                return;
        }

        ui->ln_projectname->setText(m_api->m_package);
        ui->ln_version->setText(m_api->m_version);
        ui->cb_arch->setCurrentText(m_api->m_arch);
        ui->ln_dependancies->setText(m_api->m_depends);
        ui->ln_maintainer->setText(m_api->m_maintainer);
        ui->ln_descriptiontitle->setText(m_api->m_desc_title);
        ui->txt_description->setText(m_api->m_desc_body);
        ui->ln_filesystem->setText(m_api->m_dir);
        ui->ln_outputfile->setText(m_api->m_outputfile);
        ui->ln_homepage->setText(m_api->m_homepage);
        ui->ln_replace->setText(m_api->m_replace);
        ui->ln_section->setText(m_api->m_section);
        ui->ln_source->setText(m_api->m_source);
        ui->ln_uploaders->setText(m_api->m_uploaders);
}

void MainWindow::fetch_changelog()
{
        if(ui->tab_changelog->isVisible())
                return;

        QStringList list = m_api->fetch_changelog(ui->ln_filesystem->text() + QSL("/DEBIAN/changelog"));

        if(list.isEmpty()) {
                ui->lsw_changelog->hide();
                return;
        }

        ui->lsw_changelog->show();
        ui->lsw_changelog->addItems(list);

}

void MainWindow::save_project()
{
        if(ui->ln_projectname->text().isEmpty() || ui->ln_filesystem->text().isEmpty()){
                QMessageBox::warning(this, QSL("Saving package"), QSL("Package name and directory is needed in order to work."));
                return;
        }

        m_api->m_package = ui->ln_projectname->text();
        m_api->m_version = ui->ln_version->text();
        m_api->m_dir = ui->ln_filesystem->text();
        m_api->m_outputfile = ui->ln_outputfile->text();
        ui->tab_control->setEnabled(true);
        ui->tab_changelog->setEnabled(true);
}

