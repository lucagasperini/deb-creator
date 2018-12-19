#include "form_build.h"
#include "ui_form_build.h"

#include <QFileDialog>
#include <QMessageBox>

form_build::form_build(pkgwindow *handler, QWidget *parent) :
        form_handler(handler, parent),
        ui(new Ui::form_build),
        m_model_compile(nullptr),
        m_build_db(nullptr)
{
        ui->setupUi(this);

        m_process = new multiprocess;
        m_model_compile = new QFileSystemModel;
        ui->tw_compile->setModel(m_model_compile);

        ui->ln_build_dir->setText(m_handler->m_pkg->build_dir());
        ui->ln_compile_import_remote->setText(m_handler->m_pkg->m_source);
        build_reload();

        connect(ui->btn_compile_import_local, &QPushButton::clicked, this, &form_build::compile_import_local);
        connect(ui->btn_compile_import_remote, &QPushButton::clicked, this, &form_build::compile_import_remote);
        connect(ui->btn_build, &QPushButton::clicked, this, &form_build::build);
        connect(ui->btn_buildadd, &QPushButton::clicked, this, &form_build::build_add);
        connect(ui->btn_buildremove, &QPushButton::clicked, this, &form_build::build_remove);
        connect(ui->btn_build_dir, &QPushButton::clicked, this, &form_build::build_browse_dir);
        connect(ui->btn_build_app, &QPushButton::clicked, this, &form_build::build_browse_app);
        connect(ui->btn_install_dependencies, &QPushButton::clicked, this, &form_build::install_dependencies);

        connect(ui->list_build_db, &QListWidget::currentRowChanged, this, &form_build::build_select);
        connect(m_process, &multiprocess::read, m_handler, &pkgwindow::output_append);
}

form_build::~form_build()
{
        delete ui;
}

void form_build::compile_import_local()
{
        QString local = ui->ln_compile_import_local->text();
        if(local.isEmpty()) {
                local = QFileDialog::getExistingDirectory(this, QSL("Source file of the package"));
                ui->ln_compile_import_local->setText(local);
        }

        QFileInfo dir(local);
        QString build_dir = m_handler->m_pkg->build_dir();
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

void form_build::compile_import_remote()
{
        QString remote = ui->ln_compile_import_remote->text();
        if(remote.isEmpty()) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Remote cannot be empty!\nPlease insert a remote repo."));
                return;
        }

        git* proc = new git;
        QString build_dir = m_handler->m_pkg->build_dir();
        filesystem::rmdir(build_dir);
        proc->clone(remote, build_dir, ui->ln_compile_import_branch->text());

        m_model_compile->setRootPath(build_dir);
        ui->tw_compile->setRootIndex(m_model_compile->index(build_dir));
}

void form_build::build()
{
        const int rows = ui->list_build->count();
        if(rows == 0) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Step build table cannot be empty!\nPlease insert a step build in order to compile source code."));
                return;
        }
        m_process->start();
}

void form_build::build_add()
{
        if(ui->ln_build_app->text().isEmpty()) {
                QMessageBox::warning(this, QSL("Compile Package"), QSL("Please insert a program to run in build process."));
                return;
        }
        build_step* p = new build_step;
        p->m_app = ui->ln_build_app->text();
        p->m_arg = ui->txt_build_arg->toPlainText();
        p->m_dir = ui->ln_build_dir->text();
        p->m_pkg = m_handler->m_pkg->m_id;

        if(m_build_db->find(p) == nullptr)
                m_handler->m_db->build_insert(p);

        m_process->m_build->append(p);
        ui->list_build->addItem(p->shell());
        build_reload();
}

void form_build::build_remove()
{
        const int current = ui->list_build->currentRow();
        m_process->m_build->removeAt(current);
        delete ui->list_build->item(current);
}

void form_build::build_reload()
{
        ui->list_build_db->clear();
        if(m_build_db != nullptr)
                m_build_db->clear();
        m_build_db = m_handler->m_db->build_fetch(m_handler->m_pkg->m_id);
        ui->list_build_db->addItems(m_build_db->titles());
}

void form_build::build_select(int row)
{
        if(row < 0 || row > m_build_db->size())
                return;
        build_step* current = m_build_db->at(row);
        ui->ln_build_app->setText(current->m_app);
        ui->txt_build_arg->setText(current->m_arg);
        ui->ln_build_dir->setText(current->m_dir);
}

void form_build::build_browse_app()
{
        QString program;
        program = QFileDialog::getOpenFileName(this, QSL("Select the program file"), SYS_PROGRAM_DIR);
        ui->ln_build_app->setText(program);
}

void form_build::build_browse_dir()
{
        QString dir;
        dir = QFileDialog::getExistingDirectory(this, QSL("Select the working directory"), ui->ln_build_dir->text());
        ui->ln_build_dir->setText(dir);
}

void form_build::install_dependencies()
{
        QStringList pkglist_version = m_handler->m_pkg->m_build_dep.split(',');
        QStringList pkglist;
        QString buffer;
        for(int i = 0; i < pkglist_version.size(); i++) {
                buffer = pkglist_version.at(i);
                pkglist.append(buffer.split(' ').at(0));
        }
        apt* installer = new apt;
        installer->install(pkglist);
}

void form_build::generate()
{
}
