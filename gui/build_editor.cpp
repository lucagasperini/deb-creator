#include "build_editor.h"
#include "ui_build_editor.h"

#include <define.h>

#include <QDir>
#include <QFileDialog>

build_editor::build_editor(const QList<build_step *> *steps, const QString &working_dir, QWidget *parent) :
        QDialog(parent),
        m_step(steps),
        ui(new Ui::build_editor)
{
        ui->setupUi(this);
        ui->ln_dir->setText(working_dir);

        if(steps != nullptr) {
                QStringList buffer;
                for(int i = 0; i < steps->size(); i++)
                        buffer << steps->at(i)->m_app;

                ui->list_program->addItems(buffer);
        }

        connect(ui->btn_program, &QPushButton::clicked, this, &build_editor::browse_program);
        connect(ui->btn_dir, &QPushButton::clicked, this, &build_editor::browse_dir);
        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &build_editor::ok);
        connect(ui->list_program, &QListWidget::itemSelectionChanged, this, &build_editor::list_index);
}

build_editor::~build_editor()
{
        delete ui;
}

build_step* build_editor::ok()
{
        build_step *offset = new build_step;
        offset->m_app = ui->ln_program->text();
        offset->m_arg = ui->txt_args->toPlainText();
        offset->m_dir = ui->ln_dir->text();
        return offset;
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

void build_editor::list_index()
{
        const build_step *buffer = m_step->at(ui->list_program->currentRow());
        ui->ln_program->setText(buffer->m_app);
        ui->txt_args->setText(buffer->m_arg);
        ui->ln_dir->setText(buffer->m_dir);
}
