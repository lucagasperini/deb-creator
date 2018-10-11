#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QFileInfo>

#define TAB_WELCOME 0
#define TAB_CONTROL 1

using namespace std;

mainwindow::mainwindow(QWidget *parent) :
        QMainWindow(parent),
        m_api(new debcreator),
        ui(new Ui::mainwindow),
        ui_dep(nullptr),
        ui_about(new about),
        m_model_custom(nullptr),
        m_model_compile(nullptr)
{
        ui->setupUi(this);

        ui->tabWidget->setCurrentIndex(TAB_WELCOME);

        connect(ui->btn_reload, &QPushButton::clicked, this, &mainwindow::welcome_reload);
        connect(ui->btn_add, &QPushButton::clicked, this, &mainwindow::welcome_add);
        connect(ui->btn_remove, &QPushButton::clicked, this, &mainwindow::welcome_remove);
        connect(ui->btn_dependency, &QPushButton::clicked, this, &mainwindow::depend_show);
        connect(ui->btn_gencontrol, &QPushButton::clicked, this, &mainwindow::generate_control);
        connect(ui->btn_createpackage, &QPushButton::clicked, this, &mainwindow::create_package);
        connect(ui->btn_clear, &QPushButton::clicked, this, &mainwindow::clear_output);
        connect(ui->btn_changelog_create, &QPushButton::clicked, this, &mainwindow::changelog_generate);
        connect(ui->btn_changelog_refresh, &QPushButton::clicked, this, &mainwindow::changelog_refresh);
        connect(ui->btn_refresh, &QPushButton::clicked, this, &mainwindow::compile_refresh);
        connect(ui->btn_compile, &QPushButton::clicked, this, &mainwindow::compile);
        connect(ui->btn_buildadd, &QPushButton::clicked, this, &mainwindow::build_add);
        connect(ui->btn_buildremove, &QPushButton::clicked, this, &mainwindow::build_remove);
        connect(ui->btn_buildsave, &QPushButton::clicked, this, &mainwindow::build_save);
        connect(ui->btn_custom_refresh, &QPushButton::clicked, this, &mainwindow::custom_refresh);
        connect(ui->btn_custom_save, &QPushButton::clicked, this, &mainwindow::custom_save);
        connect(ui->bt_sourcecode, &QToolButton::clicked, this, &mainwindow::dir_compile);

        connect(ui->a_create_package, &QAction::triggered, this, &mainwindow::create_package);
        connect(ui->a_generate_control, &QAction::triggered, this, &mainwindow::generate_control);
        connect(ui->a_quit, &QAction::triggered, qApp, &QApplication::quit);
        connect(ui->a_about, &QAction::triggered, ui_about, &about::show);
        connect(ui->a_aboutqt, &QAction::triggered, qApp, &QApplication::aboutQt);
        //connect(ui->tabWidget, &QTabWidget::currentChanged, this, &mainwindow::fetch_changelog);
        connect(ui->lsw_welcome, &QListWidget::currentTextChanged, this, &mainwindow::check_database);
        connect(ui->lsw_changelog, &QListWidget::currentTextChanged, this, &mainwindow::changelog_change);
        connect(ui->tree_filesystem, &QTreeView::clicked, this, &mainwindow::custom_load);
        // connect(ui->a_manual) TODO: Add a manual?

        connect(m_api->m_process, &multiprocess::read, this, &mainwindow::append_output);

        welcome_reload();
}

mainwindow::~mainwindow()
{
        delete ui;
}

void mainwindow::load(const package *pkg)
{
        ui->ln_name->setText(pkg->m_name);
        ui->ln_version->setText(pkg->m_version);
        ui->cb_arch->setCurrentText(package::architecture_name(pkg->m_arch));
        ui->ln_dependancy->setText(pkg->m_depends);
        ui->ln_maintainer->setText(pkg->m_maintainer);
        ui->ln_descriptiontitle->setText(pkg->m_desc_title);
        ui->txt_description->setText(pkg->m_desc_body);
        ui->ln_homepage->setText(pkg->m_homepage);
        ui->ln_replace->setText(pkg->m_replace);
        ui->ln_section->setText(pkg->m_section);
        ui->ln_source->setText(pkg->m_source);
        ui->ln_uploaders->setText(pkg->m_uploaders);
}

package* mainwindow::save()
{
        package *pkg = new package;

        pkg->m_name = ui->ln_name->text();
        pkg->m_version = ui->ln_version->text();
        pkg->m_arch = package::architecture_value(ui->cb_arch->currentText());
        pkg->m_depends = ui->ln_dependancy->text();
        pkg->m_maintainer = ui->ln_maintainer->text();
        pkg->m_desc_title = ui->ln_descriptiontitle->text();
        pkg->m_desc_body = ui->txt_description->toPlainText();
        pkg->m_homepage = ui->ln_homepage->text();
        pkg->m_replace = ui->ln_replace->text();
        pkg->m_section = ui->ln_section->text();
        pkg->m_source = ui->ln_source->text();
        pkg->m_uploaders = ui->ln_uploaders->text();

        return pkg;
}

void mainwindow::welcome_reload()
{
        QStringList list = m_api->db_fetch();
        ui->lsw_welcome->clear();
        ui->lsw_welcome->addItems(list);
}

void mainwindow::welcome_add()
{
        ui->tabWidget->setCurrentIndex(TAB_CONTROL);
        if(!m_api->m_pkg->is_empty()) //TODO: Manage multipackaging
                return;

        package *tmp = new package;
        tmp->m_name = QSL("Empty");
        m_api->m_pkg = tmp;
        load(tmp);
}

void mainwindow::welcome_remove()
{
        m_api->db_remove(ui->lsw_welcome->currentItem()->text());
        welcome_reload();
}

void mainwindow::depend_show()
{
        if(ui_dep == nullptr)
                ui_dep = new depend;

        ui_dep->setup(ui->ln_dependancy->text());

        if(ui_dep->exec() == QDialog::Accepted)
                ui->ln_dependancy->setText(ui_dep->ok());
}

void mainwindow::generate_control()
{
        if(ui->ln_name->text().isEmpty()) {
                QMessageBox::critical(this, QSL("Control file error"), QSL("Package name value is empty!\nPlease add a valid package name."));
                return;
        }
        if (ui->ln_name->text().contains(' ')) {
                QMessageBox::warning(this, QSL("Control file error"), QSL("Package name must not contain spaces."));
                return;
        }

        m_api->m_pkg = save();

        append_output(QSL("Generating new control file...\n"));

        if(m_api->db_insert())
                append_output(QSL("Added package into database...\n"));
        else
                append_output(QSL("Failed while adding the package to the database!\n"));

        ui->txt_control->setText(m_api->m_pkg->control());
}

void mainwindow::changelog_generate()
{
        QByteArray text = m_api->m_changelog->generate(ui->txt_changelog->toPlainText(), ui->ln_status->text(), ui->cb_urgency->currentText());
        ui->txt_changelog->setText(text);
        m_api->m_changelog->save(text);
}

void mainwindow::changelog_refresh()
{
        m_api->m_changelog->fetch();
        ui->lsw_changelog->clear();
        ui->lsw_changelog->addItems(m_api->m_changelog->titles());

}

void mainwindow::changelog_change()
{
        ui->txt_changelog->setText(m_api->m_changelog->text(ui->lsw_changelog->currentRow()));
}

void mainwindow::create_package()
{
        QString outputfile = QFileDialog::getSaveFileName(this, QSL("Select where save package"), QDir::homePath() + "/" + m_api->m_pkg->outputfile());

        if(outputfile.isEmpty()) {
                QMessageBox::warning(this, QSL("Creating Package"), QSL("Output file path cannot be empty!"));
                return;
        }

        QString control = ui->txt_control->toPlainText();

        if(control.isEmpty()) {
                QMessageBox::warning(this, QSL("Creating Package"), QSL("Control file cannot be empty!\nPlease generate a control file."));
                return;
        }

        append_output(m_api->pkg_create(control.toUtf8(), outputfile));
}

void mainwindow::append_output(const QString &text)
{
#ifdef QT_DEBUG
        qDebug() << text;
#endif
        ui->txt_output->insertPlainText(text);
}

void mainwindow::clear_output()
{
        ui->txt_output->clear();
}

void mainwindow::dir_compile()
{
        QDir dir;
        dir = QFileDialog::getExistingDirectory(this, QSL("Source file of the package"));
        ui->ln_sourcecode->setText(dir.absolutePath() + QSL("/"));
}

void mainwindow::check_database(const QString &package)
{
        if(!m_api->db_fetch(package)) {
                ui->txt_output->append(ui->ln_name->text() + QSL(" package didn't find!"));
                return;
        }

        load(m_api->m_pkg);
        ui->ln_sourcecode->setText(m_api->m_pkg->m_source);
}

void mainwindow::compile_refresh()
{
        if(ui->ln_sourcecode->text().isEmpty()) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Source cannot be empty!\nPlease insert a source code directory or git repo."));
                return;
        }

        QFileInfo dir(ui->ln_sourcecode->text());
        if(m_model_compile == nullptr)
                m_model_compile = new QFileSystemModel;
        git* proc = new git;
        QString build_dir = m_api->build_dir();

        if(dir.isDir())
                dir.absoluteDir(); //FIXME!!!
        else
                proc->clone(ui->ln_sourcecode->text(), build_dir);

#ifdef QT_DEBUG
        qDebug() << QSL("Current build path: ") << build_dir;
#endif
        m_model_compile->setRootPath(build_dir);
        ui->tw_compile->setModel(m_model_compile);
        ui->tw_compile->setRootIndex(m_model_compile->index(build_dir));
}

void mainwindow::compile()
{
        if(m_api->m_process->is_empty()) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Step build are not saved!\nPlease save step build in order to compile source code."));
                return;
        }
        m_api->m_process->start();
}

void mainwindow::build_add()
{
        const int row = ui->tbl_order->rowCount();

        ui->tbl_order->insertRow(row);
        ui->tbl_order->setItem(row, 0, new QTableWidgetItem);
        ui->tbl_order->setItem(row, 1, new QTableWidgetItem);
        ui->tbl_order->setItem(row, 2, new QTableWidgetItem);
        ui->tbl_order->item(row, 2)->setText(m_api->build_dir());
}

void mainwindow::build_remove()
{
        ui->tbl_order->removeRow(ui->tbl_order->currentRow());
}

void mainwindow::build_save()
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

void mainwindow::custom_refresh()
{
        if(m_model_custom == nullptr)
                m_model_custom = new QFileSystemModel;

        m_model_custom->setRootPath(m_api->m_pkg->root()); //+ DEBIAN/ ?
        ui->tree_filesystem->setModel(m_model_custom);
        ui->tree_filesystem->setRootIndex(m_model_custom->index(m_api->m_pkg->root()));
}

void mainwindow::custom_save()
{
        QString filename = QFileDialog::getSaveFileName(this, QSL("Select file name"), m_api->m_pkg->root());

        if(filename.isEmpty()) {
                QMessageBox::warning(this, QSL("Custom File"), QSL("File name cannot be empty!"));
                return;
        }

        debcreator::file_write(filename, ui->txt_code->toPlainText());
}


void mainwindow::custom_load(const QModelIndex &a)
{
        QString filename = m_model_custom->filePath(a);
        QByteArray text = debcreator::file_read(filename);
        ui->txt_code->setText(text);
}
