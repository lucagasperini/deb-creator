#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QFileSystemModel>
#include <QFileInfo>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
        ui->setupUi(this);

        ui->tabWidget->setCurrentIndex(0);

        connect(ui->btn_gencontrol, &QPushButton::clicked, this, &MainWindow::generate_control);
        connect(ui->btn_createpackage, &QPushButton::clicked, this, &MainWindow::create_package);
        connect(ui->btn_clear, &QPushButton::clicked, this, &MainWindow::clear_output);
        connect(ui->btn_filesystem, &QPushButton::clicked, this, &MainWindow::working_dir);
        connect(ui->btn_changelog, &QPushButton::clicked, this, &MainWindow::generate_changelog);
        connect(ui->btn_refresh, &QPushButton::clicked, this, &MainWindow::compile_refresh);
        connect(ui->btn_compile, &QPushButton::clicked, this, &MainWindow::compile);
        connect(ui->btn_buildadd, &QPushButton::clicked, this, &MainWindow::build_add);
        connect(ui->btn_buildremove, &QPushButton::clicked, this, &MainWindow::build_remove);
        connect(ui->btn_buildsave, &QPushButton::clicked, this, &MainWindow::build_save);
        connect(ui->bt_sourcecode, &QToolButton::clicked, this, &MainWindow::compile_dir);

        connect(ui->a_create_package, &QAction::triggered, this, &MainWindow::create_package);
        connect(ui->a_generate_control, &QAction::triggered, this, &MainWindow::generate_control);
        connect(ui->a_quit, &QAction::triggered, qApp, &QApplication::quit);
        connect(ui->a_about, &QAction::triggered, &about, &about::show);
        connect(ui->a_aboutqt, &QAction::triggered, qApp, &QApplication::aboutQt);
        connect(ui->ck_dependency, &QCheckBox::toggled, ui->ln_dependancies, &QLineEdit::setEnabled);
        connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::fetch_changelog);
        connect(ui->lsw_welcome, &QListWidget::currentTextChanged, this, &MainWindow::check_database);
        // connect(ui->a_manual) TODO: Add a manual?

        m_api = new debcreator();

        connect(m_api->m_process, &multiprocess::read, this, &MainWindow::append_output);

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

        QString outputfile;
        outputfile = QFileDialog::getSaveFileName(this, QSL("Select where save package"), QDir::homePath() + "/" + m_api->gen_outputfile());

        if(m_api->m_dir.isEmpty() || outputfile.isEmpty()) {
                QMessageBox::warning(this, QSL("Creating Package"), QSL("Package directory or output file path are empty!"));
                return;
        }

        QString control = ui->txt_control->toPlainText();

        if(control.isEmpty()) {
                QMessageBox::warning(this, QSL("Creating Package"), QSL("Control file cannot be empty!\nPlease generate a control file."));
                return;
        }

        append_output(m_api->package(control.toUtf8(), outputfile));
}

void MainWindow::append_output(const QByteArray &text)
{
#ifdef QT_DEBUG
        qDebug() << text;
#endif
        ui->txt_output->insertPlainText(text);
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
}

void MainWindow::compile_dir()
{
        QDir dir;
        dir = QFileDialog::getExistingDirectory(this, QSL("Source file of the package"));
        ui->ln_sourcecode->setText(dir.absolutePath() + QSL("/"));
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
        ui->ln_filesystem->setText(m_api->m_dir.path());
        ui->ln_homepage->setText(m_api->m_homepage);
        ui->ln_replace->setText(m_api->m_replace);
        ui->ln_section->setText(m_api->m_section);
        ui->ln_source->setText(m_api->m_source);
        ui->ln_uploaders->setText(m_api->m_uploaders);

        if(ui->ln_sourcecode->text().isEmpty())
                ui->ln_sourcecode->setText(m_api->m_source);
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

void MainWindow::compile_refresh()
{
        if(ui->ln_sourcecode->text().isEmpty()) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Source cannot be empty!\nPlease insert a source code directory or git repo."));
                return;
        }

        QFileInfo dir(ui->ln_sourcecode->text());
        QFileSystemModel *model = new QFileSystemModel;
        QString src_dir;

        if(dir.isDir()) {
                src_dir = dir.absoluteDir().path();
        }
        else {
                src_dir  = m_api->git_clone(ui->ln_sourcecode->text());
        }

        model->setRootPath(src_dir);
        ui->tw_compile->setModel(model);
        ui->tw_compile->setRootIndex(model->index(src_dir));
}

void MainWindow::compile()
{
        if(m_api->m_process->is_empty()) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Step build are not saved!\nPlease save step build in order to compile source code."));
                return;
        }
        m_api->m_process->start();
}

void MainWindow::build_add()
{
        const int row = ui->tbl_order->rowCount();

        ui->tbl_order->insertRow(row);
        ui->tbl_order->setItem(row, 0, new QTableWidgetItem);
        ui->tbl_order->setItem(row, 1, new QTableWidgetItem);
        ui->tbl_order->setItem(row, 2, new QTableWidgetItem);
        ui->tbl_order->item(row, 2)->setText(DEB_CREATOR_SRC);
}

void MainWindow::build_remove()
{
        ui->tbl_order->removeRow(ui->tbl_order->currentRow());
}

void MainWindow::build_save()
{
        const int rows = ui->tbl_order->rowCount();
        if(rows == 0) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Step build table cannot be empty!\nPlease insert a step build in order to compile source code."));
                return;
        }

        m_api->m_process->clear();
        for(int i = 0; i < rows; i++) {
                QString program = ui->tbl_order->item(i, 0)->text();
                QString args = ui->tbl_order->item(i, 1)->text();
                QString working_dir = ui->tbl_order->item(i, 2)->text();

                m_api->m_process->append(program, args.split(" "), working_dir);
        }
}

