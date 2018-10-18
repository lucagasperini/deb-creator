#include "build_editor.h"
#include "ui_build_editor.h"

#include <define.h>

#include <QDir>
#include <QFileDialog>

build_editor::build_editor(const QString &working_dir, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::build_editor)
{
        ui->setupUi(this);
        ui->ln_dir->setText(working_dir);

        connect(ui->btn_program, &QPushButton::clicked, this, &build_editor::browse_program);
        connect(ui->btn_dir, &QPushButton::clicked, this, &build_editor::browse_dir);
        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &build_editor::ok);
}

build_editor::~build_editor()
{
        delete ui;
}

void build_editor::ok()
{
        program = ui->ln_program->text();
        args = ui->txt_args->toPlainText();
        working_dir = ui->ln_dir->text();
}

void build_editor::browse_program()
{
        QString program;
        program = QFileDialog::getOpenFileName(this, QSL("Select the program file"), SYS_PROGRAM_DIR);
        ui->ln_program->setText(program);
}

void build_editor::browse_dir()
{
        QString dir;
        dir = QFileDialog::getExistingDirectory(this, QSL("Select the working directory"), ui->ln_dir->text());
        ui->ln_dir->setText(dir);
}
