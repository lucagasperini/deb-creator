#include "form_custom.h"
#include "ui_form_custom.h"

#include <QFileDialog>
#include <QMessageBox>

form_custom::form_custom(pkgwindow *handler, QWidget *parent) :
        form_handler(handler, parent),
        ui(new Ui::form_custom),
        m_model_custom(nullptr)
{
        ui->setupUi(this);

        m_model_custom = new QFileSystemModel;
        ui->tree_filesystem->setModel(m_model_custom);

        connect(ui->btn_custom_refresh, &QPushButton::clicked, this, &form_custom::custom_refresh);
        connect(ui->btn_custom_save, &QPushButton::clicked, this, &form_custom::custom_save);

        connect(ui->tree_filesystem, &QTreeView::clicked, this, &form_custom::custom_load);
}

form_custom::~form_custom()
{
        delete ui;
}

void form_custom::custom_refresh()
{
        m_model_custom->setRootPath(m_handler->m_pkg->root()); //+ DEBIAN/ ?
        ui->tree_filesystem->setRootIndex(m_model_custom->index(m_handler->m_pkg->root()));
}

void form_custom::custom_save()
{
        QString filename = QFileDialog::getSaveFileName(this, QSL("Select file name"), m_handler->m_pkg->root());

        if(filename.isEmpty()) {
                QMessageBox::warning(this, QSL("Custom File"), QSL("File name cannot be empty!"));
                return;
        }

        filesystem::file_write(filename, ui->txt_code->toPlainText().toUtf8());
}


void form_custom::custom_load(const QModelIndex &a)
{
        QString filename = m_model_custom->filePath(a);
        QByteArray text = filesystem::file_read(filename);
        ui->txt_code->setText(text);
}

void form_custom::generate()
{
}
