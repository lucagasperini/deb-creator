#include "pkgwindow.h"
#include "ui_pkgwindow.h"

#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QFileInfo>

#define TAB_CONTROL 0
#define TAB_COMPILE 1
#define TAB_CHANGELOG 2
#define TAB_CUSTOM 3

using namespace std;

pkgwindow::pkgwindow(const package &pkg, QWidget *parent) :
        QMainWindow(parent),
        m_build_db(nullptr),
        ui(new Ui::pkgwindow),
        ui_dep(nullptr),
        m_model_custom(nullptr),
        m_model_compile(nullptr)
{
        m_pkg = new package(pkg);
        m_process = new multiprocess;
        m_db = new database(DEB_CREATOR_DB, m_pkg->m_name);

        ui->setupUi(this);

        m_model_compile = new QFileSystemModel;
        ui->tw_compile->setModel(m_model_compile);
        m_model_custom = new QFileSystemModel;
        ui->tree_filesystem->setModel(m_model_custom);

        connect(ui->btn_dependency, &QPushButton::clicked, this, &pkgwindow::depend_show);
        connect(ui->btn_build_dep, &QPushButton::clicked, this, &pkgwindow::depend_build_show);
        connect(ui->btn_gencontrol, &QPushButton::clicked, this, &pkgwindow::control_generate);
        connect(ui->btn_createpackage, &QPushButton::clicked, this, &pkgwindow::package_generate);
        connect(ui->btn_clear, &QPushButton::clicked, this, &pkgwindow::output_clear);
        connect(ui->btn_changelog_add, &QPushButton::clicked, this, &pkgwindow::changelog_add);
        connect(ui->btn_changelog_remove, &QPushButton::clicked, this, &pkgwindow::changelog_remove);
        connect(ui->btn_changelog_save, &QPushButton::clicked, this, &pkgwindow::changelog_save);
        connect(ui->btn_compile_import_local, &QPushButton::clicked, this, &pkgwindow::compile_import_local);
        connect(ui->btn_compile_import_remote, &QPushButton::clicked, this, &pkgwindow::compile_import_remote);
        connect(ui->btn_build, &QPushButton::clicked, this, &pkgwindow::build);
        connect(ui->btn_buildadd, &QPushButton::clicked, this, &pkgwindow::build_add);
        connect(ui->btn_buildremove, &QPushButton::clicked, this, &pkgwindow::build_remove);
        connect(ui->btn_build_dir, &QPushButton::clicked, this, &pkgwindow::build_browse_dir);
        connect(ui->btn_build_app, &QPushButton::clicked, this, &pkgwindow::build_browse_app);
        connect(ui->btn_custom_refresh, &QPushButton::clicked, this, &pkgwindow::custom_refresh);
        connect(ui->btn_custom_save, &QPushButton::clicked, this, &pkgwindow::custom_save);

        connect(ui->lsw_changelog, &QListWidget::currentRowChanged, this, &pkgwindow::changelog_change);
        connect(ui->list_build_db, &QListWidget::currentRowChanged, this, &pkgwindow::build_select);
        connect(ui->tree_filesystem, &QTreeView::clicked, this, &pkgwindow::custom_load);
        // connect(ui->a_manual) TODO: Add a manual?

        connect(m_process, &multiprocess::read, this, &pkgwindow::output_append);

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

        ui->ln_build_dir->setText(m_pkg->build_dir());

        changelog_reload();
        build_reload();
}

pkgwindow::~pkgwindow()
{
        delete ui;
}

void pkgwindow::save()
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

void pkgwindow::depend_show()
{
        if(ui_dep == nullptr)
                ui_dep = new depend;

        ui_dep->setup(ui->ln_dependancy->text());

        if(ui_dep->exec() == QDialog::Accepted)
                ui->ln_dependancy->setText(ui_dep->ok());
}

void pkgwindow::depend_build_show()
{
        if(ui_dep == nullptr)
                ui_dep = new depend;

        ui_dep->setup(ui->ln_build_dep->text());

        if(ui_dep->exec() == QDialog::Accepted)
                ui->ln_build_dep->setText(ui_dep->ok());
}

void pkgwindow::control_generate()
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

void pkgwindow::changelog_add()
{
        changelog cl(0, m_pkg, "", "New changelog", "", "");
        m_db->cl_insert(cl);
        changelog_reload();
}

void pkgwindow::changelog_remove()
{
        changelog *current = m_changelog->at(ui->lsw_changelog->currentRow());
        m_db->cl_remove(current->m_id);
        changelog_reload();
}

void pkgwindow::changelog_save()
{
        changelog cl(0, m_pkg, ui->ln_cl_version->text(), ui->txt_changelog->toPlainText().toUtf8(), ui->ln_status->text(), ui->cb_urgency->currentText()); //FIXME: "0" AS INVALID ID???
        changelog *current = m_changelog->at(ui->lsw_changelog->currentRow());
        m_db->cl_update(current->m_id, cl);
        changelog_reload();
}

void pkgwindow::changelog_reload()
{
        m_changelog = m_db->cl_fetch(m_pkg);
        ui->lsw_changelog->clear();
        if(m_changelog == nullptr || m_changelog->isEmpty()) {
                return;
        }
        QStringList titles;
        for(int i = 0; i < m_changelog->size(); i++)
                titles << m_changelog->at(i)->title();
        ui->lsw_changelog->addItems(titles);
}

void pkgwindow::changelog_change(int row)
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

void pkgwindow::package_generate()
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
        dpkg* create = new dpkg(m_pkg->root());
        create->changelogs(m_changelog);
        create->control(control.toUtf8());
        output_append(create->exec(outputfile));
}

void pkgwindow::output_append(const QString &text)
{
#ifdef QT_DEBUG
        qDebug() << text;
#endif
        ui->txt_output->insertPlainText(text);
}

void pkgwindow::output_clear()
{
        ui->txt_output->clear();
}


void pkgwindow::compile_import_local()
{
        QString local = ui->ln_compile_import_local->text();
        if(local.isEmpty()) {
                local = QFileDialog::getExistingDirectory(this, QSL("Source file of the package"));
                ui->ln_compile_import_local->setText(local);
        }

        QFileInfo dir(local);
        QString build_dir = m_pkg->build_dir();
        filesystem::rmdir(build_dir);

        if(dir.isDir()) {
                filesystem::cp(local, build_dir);
        } else {
                QMessageBox::warning(this, QSL("Compile Import from local"), QSL("Invalid local path!"));
                return;
        }

        m_model_compile->setRootPath(build_dir);
        ui->tw_compile->setRootIndex(m_model_compile->index(build_dir));
}

void pkgwindow::compile_import_remote()
{
        QString remote = ui->ln_compile_import_remote->text();
        if(remote.isEmpty()) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Remote cannot be empty!\nPlease insert a remote repo."));
                return;
        }

        git* proc = new git;
        QString build_dir = m_pkg->build_dir();
        filesystem::rmdir(build_dir);
        proc->clone(remote, build_dir);

        m_model_compile->setRootPath(build_dir);
        ui->tw_compile->setRootIndex(m_model_compile->index(build_dir));
}

void pkgwindow::build()
{
        const int rows = ui->list_build->count();
        if(rows == 0) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Step build table cannot be empty!\nPlease insert a step build in order to compile source code."));
                return;
        }
        m_process->start();
}

void pkgwindow::build_add()
{
        if(ui->ln_build_app->text().isEmpty()) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Please insert a program to run in build process."));
                return;
        }
        build_step* p = new build_step;
        p->m_app = ui->ln_build_app->text();
        p->m_arg = ui->txt_build_arg->toPlainText();
        p->m_dir = ui->ln_build_dir->text();
        p->m_pkg = m_pkg->m_id;

        const int current = ui->list_build_db->currentRow();
        if(current > 0) {
                build_step* t = m_build_db->at(current);
                if(p->m_app != t->m_app || p->m_arg != t->m_arg || p->m_dir != t->m_dir)
                        m_db->build_update(t->m_id, *p);
        } else {
                m_db->build_insert(*p);
        }
        m_process->m_build->append(p);
        QString text = "[ " + p->m_dir + " ]$ " + p->m_app + " " + p->m_arg;
        ui->list_build->addItem(text);
        build_reload();
}

void pkgwindow::build_remove()
{
        const int current = ui->list_build->currentRow();
        m_process->m_build->removeAt(current);
        delete ui->list_build->item(current);
}

void pkgwindow::build_reload()
{
        ui->list_build_db->clear();
        if(m_build_db != nullptr)
                m_build_db->clear();
        m_build_db = m_db->build_fetch(m_pkg->m_id);
        for(int i = 0; i < m_build_db->size(); i++)
                ui->list_build_db->addItem(m_build_db->at(i)->m_app);
}

void pkgwindow::build_select(int row)
{
        if(row < 0 || row > m_build_db->size())
                return;
        build_step* current = m_build_db->at(row);
        ui->ln_build_app->setText(current->m_app);
        ui->txt_build_arg->setText(current->m_arg);
        ui->ln_build_dir->setText(current->m_dir);
}

void pkgwindow::build_browse_app()
{
        QString program;
        program = QFileDialog::getOpenFileName(this, QSL("Select the program file"), SYS_PROGRAM_DIR);
        ui->ln_build_app->setText(program);
}

void pkgwindow::build_browse_dir()
{
        QString dir;
        dir = QFileDialog::getExistingDirectory(this, QSL("Select the working directory"), ui->ln_build_dir->text());
        ui->ln_build_dir->setText(dir);
}

void pkgwindow::custom_refresh()
{
        m_model_custom->setRootPath(m_pkg->root()); //+ DEBIAN/ ?
        ui->tree_filesystem->setRootIndex(m_model_custom->index(m_pkg->root()));
}

void pkgwindow::custom_save()
{
        QString filename = QFileDialog::getSaveFileName(this, QSL("Select file name"), m_pkg->root());

        if(filename.isEmpty()) {
                QMessageBox::warning(this, QSL("Custom File"), QSL("File name cannot be empty!"));
                return;
        }

        filesystem::file_write(filename, ui->txt_code->toPlainText().toUtf8());
}


void pkgwindow::custom_load(const QModelIndex &a)
{
        QString filename = m_model_custom->filePath(a);
        QByteArray text = filesystem::file_read(filename);
        ui->txt_code->setText(text);
}
