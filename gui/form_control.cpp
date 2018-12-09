#include "form_control.h"
#include "ui_form_control.h"

#include <QMessageBox>

form_control::form_control(pkgwindow *handler, QWidget *parent) :
        form_handler(handler, parent),
        ui(new Ui::form_control),
        ui_dep(nullptr)
{
        ui->setupUi(this);

        ui->ln_name->setText(m_handler->m_pkg->m_name);
        ui->ln_version->setText(m_handler->m_pkg->m_version);
        ui->cb_arch->setCurrentText(package::architecture_name(m_handler->m_pkg->m_arch));
        ui->ln_dependancy->setText(m_handler->m_pkg->m_depends);
        ui->ln_build_dep->setText(m_handler->m_pkg->m_build_dep);
        ui->ln_maintainer->setText(m_handler->m_pkg->m_maintainer);
        ui->ln_descriptiontitle->setText(m_handler->m_pkg->m_desc_title);
        ui->txt_description->setText(m_handler->m_pkg->m_desc_body);
        ui->ln_homepage->setText(m_handler->m_pkg->m_homepage);
        ui->ln_replace->setText(m_handler->m_pkg->m_replace);
        ui->ln_section->setText(m_handler->m_pkg->m_section);
        ui->ln_source->setText(m_handler->m_pkg->m_source);
        ui->ln_uploaders->setText(m_handler->m_pkg->m_uploaders);

        connect(ui->btn_dependency, &QPushButton::clicked, this, &form_control::depend_show);
        connect(ui->btn_build_dep, &QPushButton::clicked, this, &form_control::depend_build_show);
        connect(ui->btn_gencontrol, &QPushButton::clicked, this, &form_control::control_generate);
}

form_control::~form_control()
{
        delete ui;
}

void form_control::save()
{
        m_handler->m_pkg->m_name = ui->ln_name->text();
        m_handler->m_pkg->m_version = ui->ln_version->text();
        m_handler->m_pkg->m_arch = package::architecture_value(ui->cb_arch->currentText());
        m_handler->m_pkg->m_depends = ui->ln_dependancy->text();
        m_handler->m_pkg->m_build_dep = ui->ln_build_dep->text();
        m_handler->m_pkg->m_maintainer = ui->ln_maintainer->text();
        m_handler->m_pkg->m_desc_title = ui->ln_descriptiontitle->text();
        m_handler->m_pkg->m_desc_body = ui->txt_description->toPlainText();
        m_handler->m_pkg->m_homepage = ui->ln_homepage->text();
        m_handler->m_pkg->m_replace = ui->ln_replace->text();
        m_handler->m_pkg->m_section = ui->ln_section->text();
        m_handler->m_pkg->m_source = ui->ln_source->text();
        m_handler->m_pkg->m_uploaders = ui->ln_uploaders->text();
}

void form_control::depend_show()
{
        if(ui_dep == nullptr)
                ui_dep = new depend;

        ui_dep->setup(ui->ln_dependancy->text());

        if(ui_dep->exec() == QDialog::Accepted)
                ui->ln_dependancy->setText(ui_dep->ok());
}

void form_control::depend_build_show()
{
        if(ui_dep == nullptr)
                ui_dep = new depend;

        ui_dep->setup(ui->ln_build_dep->text());

        if(ui_dep->exec() == QDialog::Accepted)
                ui->ln_build_dep->setText(ui_dep->ok());
}

void form_control::control_generate()
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

        m_handler->output_append(QSL("Generating new control file...\n"));

        if(m_handler->m_db->pkg_insert(m_handler->m_pkg))
                m_handler->output_append(QSL("Added package into database...\n"));
        else
                m_handler->output_append(QSL("Failed while adding the package to the database!\n"));

        ui->txt_control->setText(m_handler->m_pkg->control());
}

void form_control::generate()
{
        QString control = ui->txt_control->toPlainText();

        if(control.isEmpty()) {
                QMessageBox::warning(this, QSL("Creating Package"), QSL("Control file cannot be empty!\nPlease generate a control file."));
                return;
        }
        m_handler->m_dpkg->control(control.toUtf8());
}
