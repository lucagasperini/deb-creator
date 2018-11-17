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
        ui(new Ui::mainwindow),
        ui_dep(nullptr),
        ui_about(new about),
        m_model_custom(nullptr),
        m_model_compile(nullptr)
{
        m_pkg = new package;
        m_process = new multiprocess;
        m_db = new database;
        filesystem::debcreator_directory();

        ui->setupUi(this);

        ui->tabWidget->setCurrentIndex(TAB_WELCOME);

        connect(ui->btn_reload, &QPushButton::clicked, this, &mainwindow::welcome_reload);
        connect(ui->btn_add, &QPushButton::clicked, this, &mainwindow::welcome_add);
        connect(ui->btn_remove, &QPushButton::clicked, this, &mainwindow::welcome_remove);
        connect(ui->btn_dependency, &QPushButton::clicked, this, &mainwindow::depend_show);
        connect(ui->btn_build_dep, &QPushButton::clicked, this, &mainwindow::depend_build_show);
        connect(ui->btn_gencontrol, &QPushButton::clicked, this, &mainwindow::control_generate);
        connect(ui->btn_createpackage, &QPushButton::clicked, this, &mainwindow::package_generate);
        connect(ui->btn_clear, &QPushButton::clicked, this, &mainwindow::output_clear);
        connect(ui->btn_changelog_add, &QPushButton::clicked, this, &mainwindow::changelog_add);
        connect(ui->btn_changelog_remove, &QPushButton::clicked, this, &mainwindow::changelog_remove);
        connect(ui->btn_changelog_save, &QPushButton::clicked, this, &mainwindow::changelog_save);
        connect(ui->btn_refresh, &QPushButton::clicked, this, &mainwindow::compile_refresh);
        connect(ui->btn_compile, &QPushButton::clicked, this, &mainwindow::compile);
        connect(ui->btn_buildadd, &QPushButton::clicked, this, &mainwindow::build_add);
        connect(ui->btn_buildremove, &QPushButton::clicked, this, &mainwindow::build_remove);
        connect(ui->btn_buildsave, &QPushButton::clicked, this, &mainwindow::build_save);
        connect(ui->btn_custom_refresh, &QPushButton::clicked, this, &mainwindow::custom_refresh);
        connect(ui->btn_custom_save, &QPushButton::clicked, this, &mainwindow::custom_save);
        connect(ui->bt_sourcecode, &QToolButton::clicked, this, &mainwindow::compile_directory);

        connect(ui->a_create_package, &QAction::triggered, this, &mainwindow::package_generate);
        connect(ui->a_generate_control, &QAction::triggered, this, &mainwindow::control_generate);
        connect(ui->a_quit, &QAction::triggered, qApp, &QApplication::quit);
        connect(ui->a_about, &QAction::triggered, ui_about, &about::show);
        connect(ui->a_aboutqt, &QAction::triggered, qApp, &QApplication::aboutQt);
        connect(ui->a_loadctrl, &QAction::triggered, this, &mainwindow::control_load);
        connect(ui->lsw_welcome, &QListWidget::currentTextChanged, this, &mainwindow::control_database);
        connect(ui->lsw_changelog, &QListWidget::currentRowChanged, this, &mainwindow::changelog_change);
        connect(ui->tree_filesystem, &QTreeView::clicked, this, &mainwindow::custom_load);
        // connect(ui->a_manual) TODO: Add a manual?

        connect(m_process, &multiprocess::read, this, &mainwindow::output_append);

        welcome_reload();
}

mainwindow::~mainwindow()
{
        delete ui;
}

void mainwindow::load()
{
        ui->ln_name->setText(m_pkg->m_name);
        ui->ln_version->setText(m_pkg->m_version);
        ui->cb_arch->setCurrentText(package::architecture_name(m_pkg->m_arch));
        ui->ln_dependancy->setText(m_pkg->m_depends);
        ui->ln_build_dep->setText(m_pkg->m_build_dep);
        ui->ln_maintainer->setText(m_pkg->m_maintainer);
        ui->ln_descriptiontitle->setText(m_pkg->m_desc_title);
        ui->txt_description->setText(m_pkg->m_desc_body);
        ui->ln_homepage->setText(m_pkg->m_homepage);
        ui->ln_replace->setText(m_pkg->m_replace);
        ui->ln_section->setText(m_pkg->m_section);
        ui->ln_source->setText(m_pkg->m_source);
        ui->ln_uploaders->setText(m_pkg->m_uploaders);

        ui->ln_sourcecode->setText(m_pkg->m_source);
}

void mainwindow::save()
{
        m_pkg->m_name = ui->ln_name->text();
        m_pkg->m_version = ui->ln_version->text();
        m_pkg->m_arch = package::architecture_value(ui->cb_arch->currentText());
        m_pkg->m_depends = ui->ln_dependancy->text();
        m_pkg->m_build_dep = ui->ln_build_dep->text();
        m_pkg->m_maintainer = ui->ln_maintainer->text();
        m_pkg->m_desc_title = ui->ln_descriptiontitle->text();
        m_pkg->m_desc_body = ui->txt_description->toPlainText();
        m_pkg->m_homepage = ui->ln_homepage->text();
        m_pkg->m_replace = ui->ln_replace->text();
        m_pkg->m_section = ui->ln_section->text();
        m_pkg->m_source = ui->ln_source->text();
        m_pkg->m_uploaders = ui->ln_uploaders->text();
}

void mainwindow::welcome_reload()
{
        m_pkglist = m_db->pkg_fetch();
        if(m_pkglist == nullptr)
                return;

        ui->lsw_welcome->clear();
        ui->lsw_welcome->addItems(m_pkglist->values());
}

void mainwindow::welcome_add()
{
        ui->tabWidget->setCurrentIndex(TAB_CONTROL);
        if(!m_pkg->is_empty()) //TODO: Manage multipackaging
                return;

        m_pkg->m_name = QSL("Empty");
        load();
}

void mainwindow::welcome_remove()
{
        QString name = ui->lsw_welcome->currentItem()->text();
        m_db->pkg_remove(m_pkglist->key(name));
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

void mainwindow::depend_build_show()
{
        if(ui_dep == nullptr)
                ui_dep = new depend;

        ui_dep->setup(ui->ln_build_dep->text());

        if(ui_dep->exec() == QDialog::Accepted)
                ui->ln_build_dep->setText(ui_dep->ok());
}

void mainwindow::control_generate()
{
        if(ui->ln_name->text().isEmpty()) {
                QMessageBox::critical(this, QSL("Control file error"), QSL("Package name value is empty!\nPlease add a valid package name."));
                return;
        }
        if (ui->ln_name->text().contains(' ')) {
                QMessageBox::warning(this, QSL("Control file error"), QSL("Package name must not contain spaces."));
                return;
        }

        save();

        output_append(QSL("Generating new control file...\n"));

        if(m_db->pkg_insert(m_pkg))
                output_append(QSL("Added package into database...\n"));
        else
                output_append(QSL("Failed while adding the package to the database!\n"));

        ui->txt_control->setText(m_pkg->control());
}

void mainwindow::changelog_add()
{
        changelog cl(0, m_pkg, "", "New changelog", "", "");
        m_db->cl_insert(cl);
        changelog_reload();
}

void mainwindow::changelog_remove()
{
        changelog *current = m_changelog->at(ui->lsw_changelog->currentRow());
        m_db->cl_remove(current->m_id);
        changelog_reload();
}

void mainwindow::changelog_save()
{
        changelog cl(0, m_pkg, ui->ln_cl_version->text(), ui->txt_changelog->toPlainText().toUtf8(), ui->ln_status->text(), ui->cb_urgency->currentText()); //FIXME: "0" AS INVALID ID???
        changelog *current = m_changelog->at(ui->lsw_changelog->currentRow());
        m_db->cl_update(current->m_id, cl);
        changelog_reload();
}

void mainwindow::changelog_reload()
{
        m_changelog = m_db->cl_fetch(m_pkg);
        if(m_changelog == nullptr || m_changelog->isEmpty())
                return;
        QStringList titles;
        for(int i = 0; i < m_changelog->size(); i++)
                titles << m_changelog->at(i)->title();
        ui->lsw_changelog->clear();
        ui->lsw_changelog->addItems(titles);
}

void mainwindow::changelog_change(int row)
{
        if(row == -1) {
                row = m_changelog->size() - 1;
                ui->lsw_changelog->setCurrentRow(row); //if row is invalid select last item
        }
        changelog* selected = m_changelog->at(row);
        ui->txt_changelog->setText(selected->m_text);
        ui->ln_status->setText(selected->m_status);
        ui->cb_urgency->setCurrentText(selected->m_urgency);
        ui->ln_cl_version->setText(selected->m_version);
}

void mainwindow::package_generate()
{
        QString outputfile = QFileDialog::getSaveFileName(this, QSL("Select where save package"), QDir::homePath() + "/" + m_pkg->outputfile());

        if(outputfile.isEmpty()) {
                QMessageBox::warning(this, QSL("Creating Package"), QSL("Output file path cannot be empty!"));
                return;
        }

        QString control = ui->txt_control->toPlainText();

        if(control.isEmpty()) {
                QMessageBox::warning(this, QSL("Creating Package"), QSL("Control file cannot be empty!\nPlease generate a control file."));
                return;
        }

        if(m_changelog != nullptr && m_changelog->isEmpty()) {
                QByteArray cl_text;
                for(int i = 0; i < m_changelog->size(); i++) {
                        cl_text.append(m_changelog->at(i)->generate());
                }
                filesystem::file_write(m_pkg->root() + QSL("/DEBIAN/changelog"), cl_text);
        }
        output_append(m_pkg->create(control.toUtf8(), outputfile)); //TODO: ADD DPKG CLASS TO HANDLE PACKAGE CREATION
}

void mainwindow::output_append(const QString &text)
{
#ifdef QT_DEBUG
        qDebug() << text;
#endif
        ui->txt_output->insertPlainText(text);
}

void mainwindow::output_clear()
{
        ui->txt_output->clear();
}

void mainwindow::compile_directory()
{
        QDir dir;
        dir = QFileDialog::getExistingDirectory(this, QSL("Source file of the package"));
        ui->ln_sourcecode->setText(dir.absolutePath() + QSL("/"));
}

void mainwindow::control_database(const QString &str)
{
        package* tmp = m_db->pkg_fetch(m_pkglist->key(str));
        if(tmp == nullptr || tmp->is_empty()) {
                ui->txt_output->append(str + QSL(" package didn't find!"));
                return;
        }

        m_pkg->copy(*tmp);
        load();
        changelog_reload();
}

void mainwindow::control_load()
{
        QString filename = QFileDialog::getOpenFileName(this, QSL("Select a valid control file"), QDir::homePath());
        if(filename.isEmpty()) {
                QMessageBox::warning(this, QSL("Control File"), QSL("File name cannot be empty!"));
                return;
        }
        package *tmp = new package(filesystem::file_read(filename));
        if(tmp == nullptr || tmp->is_empty()) {
                ui->txt_output->append(filename + QSL(" control file is not valid!"));
                return;
        }
        m_pkg->copy(*tmp);
        load();
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
        QString build_dir = m_pkg->build_dir();

        if(dir.isDir())
                filesystem::cp(dir.absoluteDir().path(), build_dir);
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
        if(m_process->is_empty()) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Step build are not saved!\nPlease save step build in order to compile source code."));
                return;
        }
        m_process->start();
}

void mainwindow::build_add()
{
        const int row = ui->tbl_order->rowCount();

        build_editor *editor = new build_editor(m_db->build_fetch(m_pkg->m_name), m_pkg->build_dir(), this);
        if(editor->exec() == QDialog::Rejected)
                return;

        editor->ok();

        ui->tbl_order->insertRow(row);
        ui->tbl_order->setItem(row, 0, new QTableWidgetItem(editor->program));
        ui->tbl_order->setItem(row, 1, new QTableWidgetItem(editor->args));
        ui->tbl_order->setItem(row, 2, new QTableWidgetItem(editor->working_dir));
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

        m_process->clear();
        for(int i = 0; i < rows; i++) {
                build_step step;
                step.program = ui->tbl_order->item(i, 0)->text();
                step.argument = ui->tbl_order->item(i, 1)->text();
                step.directory = ui->tbl_order->item(i, 2)->text();

                m_process->append(step);
                m_db->build_insert(m_pkg->m_name, step);
        }
}

void mainwindow::custom_refresh()
{
        if(m_model_custom == nullptr)
                m_model_custom = new QFileSystemModel;

        m_model_custom->setRootPath(m_pkg->root()); //+ DEBIAN/ ?
        ui->tree_filesystem->setModel(m_model_custom);
        ui->tree_filesystem->setRootIndex(m_model_custom->index(m_pkg->root()));
}

void mainwindow::custom_save()
{
        QString filename = QFileDialog::getSaveFileName(this, QSL("Select file name"), m_pkg->root());

        if(filename.isEmpty()) {
                QMessageBox::warning(this, QSL("Custom File"), QSL("File name cannot be empty!"));
                return;
        }

        filesystem::file_write(filename, ui->txt_code->toPlainText().toUtf8());
}


void mainwindow::custom_load(const QModelIndex &a)
{
        QString filename = m_model_custom->filePath(a);
        QByteArray text = filesystem::file_read(filename);
        ui->txt_code->setText(text);
}
