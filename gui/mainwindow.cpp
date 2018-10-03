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

#define TAB_WELCOME 0
#define TAB_CONTROL 1

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        m_api(new debcreator),
        ui(new Ui::MainWindow),
        ui_dep(nullptr),
        ui_about(new about)
{
        ui->setupUi(this);

        ui->tabWidget->setCurrentIndex(TAB_WELCOME);

        connect(ui->btn_reload, &QPushButton::clicked, this, &MainWindow::welcome_reload);
        connect(ui->btn_add, &QPushButton::clicked, this, &MainWindow::welcome_add);
        connect(ui->btn_remove, &QPushButton::clicked, this, &MainWindow::welcome_remove);
        connect(ui->btn_dependency, &QPushButton::clicked, this, &MainWindow::depend_show);
        connect(ui->btn_gencontrol, &QPushButton::clicked, this, &MainWindow::generate_control);
        connect(ui->btn_createpackage, &QPushButton::clicked, this, &MainWindow::create_package);
        connect(ui->btn_clear, &QPushButton::clicked, this, &MainWindow::clear_output);
        connect(ui->btn_changelog_create, &QPushButton::clicked, this, &MainWindow::changelog_generate);
        connect(ui->btn_changelog_refresh, &QPushButton::clicked, this, &MainWindow::changelog_refresh);
        connect(ui->btn_refresh, &QPushButton::clicked, this, &MainWindow::compile_refresh);
        connect(ui->btn_compile, &QPushButton::clicked, this, &MainWindow::compile);
        connect(ui->btn_buildadd, &QPushButton::clicked, this, &MainWindow::build_add);
        connect(ui->btn_buildremove, &QPushButton::clicked, this, &MainWindow::build_remove);
        connect(ui->btn_buildsave, &QPushButton::clicked, this, &MainWindow::build_save);
        connect(ui->bt_sourcecode, &QToolButton::clicked, this, &MainWindow::compile_dir);

        connect(ui->a_create_package, &QAction::triggered, this, &MainWindow::create_package);
        connect(ui->a_generate_control, &QAction::triggered, this, &MainWindow::generate_control);
        connect(ui->a_quit, &QAction::triggered, qApp, &QApplication::quit);
        connect(ui->a_about, &QAction::triggered, ui_about, &about::show);
        connect(ui->a_aboutqt, &QAction::triggered, qApp, &QApplication::aboutQt);
        //connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::fetch_changelog);
        connect(ui->lsw_welcome, &QListWidget::currentTextChanged, this, &MainWindow::check_database);
        // connect(ui->a_manual) TODO: Add a manual?

        connect(m_api->m_process, &multiprocess::read, this, &MainWindow::append_output);

        welcome_reload();
}

MainWindow::~MainWindow()
{
        delete ui;
}

void MainWindow::welcome_reload()
{
        QStringList list = m_api->db_fetch();
        ui->lsw_welcome->clear();
        ui->lsw_welcome->addItems(list);
}

void MainWindow::welcome_add()
{
        ui->tabWidget->setCurrentIndex(TAB_CONTROL);
        if(!m_api->m_pkg->is_empty()) //TODO: Manage multipackaging
                return;

        QDir dir;
        dir = QFileDialog::getExistingDirectory(this, QSL("Select package directory")); //TODO: Manage invalid path
        ui->ln_sourcecode->setText(dir.absolutePath());

        package *tmp = new package;
        tmp->m_name = QSL("Empty");
        tmp->m_dir = dir;
        m_api->m_pkg = tmp;
        ui->ui_package->load(*tmp);

}

void MainWindow::welcome_remove()
{
        m_api->db_remove(ui->lsw_welcome->currentItem()->text());
        welcome_reload();
}

void MainWindow::depend_show()
{
        if(ui_dep == nullptr)
                ui_dep = new depend;
        ui_dep->show();
}

void MainWindow::generate_control()
{
        if(!ui->ui_package->check())
                return;

        append_output(QSL("Generating new control file..."));

        if(m_api->db_insert())
                append_output(QSL("Added package into database..."));
        else
                append_output(QSL("Failed while adding the package to the database!"));

        ui->txt_control->setText(m_api->m_pkg->control());
}

void MainWindow::changelog_generate()
{
        m_api->m_changelog->generate(ui->txt_changelog->toPlainText(), ui->ln_status->text(), ui->cb_urgency->currentText());
        ui->txt_changelog->setText(m_api->m_changelog->m_text);
        m_api->m_changelog->save();
}

void MainWindow::changelog_refresh()
{
        QStringList list = m_api->m_changelog->fetch();

        ui->lsw_changelog->show();
        ui->lsw_changelog->clear();
        ui->lsw_changelog->addItems(list);

}

void MainWindow::create_package()
{
        QString outputfile = QFileDialog::getSaveFileName(this, QSL("Select where save package"), QDir::homePath() + "/" + m_api->gen_outputfile());

        if(m_api->m_pkg->m_dir.isEmpty() || outputfile.isEmpty()) {
                QMessageBox::warning(this, QSL("Creating Package"), QSL("Package directory or output file path are empty!"));
                return;
        }

        QString control = ui->txt_control->toPlainText();

        if(control.isEmpty()) {
                QMessageBox::warning(this, QSL("Creating Package"), QSL("Control file cannot be empty!\nPlease generate a control file."));
                return;
        }

        append_output(m_api->pkg_create(control.toUtf8(), outputfile));
}

void MainWindow::append_output(const QString &text)
{
#ifdef QT_DEBUG
        qDebug() << text;
#endif
        ui->txt_output->insertPlainText(text);
}

void MainWindow::clear_output()
{
        ui->txt_output->clear();
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
                ui->txt_output->append(ui->ui_package->ln_name->text() + QSL(" package didn't find!"));
                return;
        }

        ui->ui_package->load(*m_api->m_pkg);
        ui->ln_sourcecode->setText(m_api->m_pkg->m_source);
}

void MainWindow::compile_refresh()
{
        if(ui->ln_sourcecode->text().isEmpty()) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Source cannot be empty!\nPlease insert a source code directory or git repo."));
                return;
        }

        QFileInfo dir(ui->ln_sourcecode->text());
        QFileSystemModel *model = new QFileSystemModel;

        if(dir.isDir())
                m_api->m_build = dir.absoluteDir().path();
        else
                m_api->m_build = DEB_CREATOR_SRC + m_api->m_git->clone(ui->ln_sourcecode->text());

        model->setRootPath(m_api->m_build);
        ui->tw_compile->setModel(model);
        ui->tw_compile->setRootIndex(model->index(m_api->m_build));
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
        ui->tbl_order->item(row, 2)->setText(m_api->m_build);
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

