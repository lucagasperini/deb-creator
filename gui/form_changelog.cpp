#include "form_changelog.h"
#include "ui_form_changelog.h"

#include <QMessageBox>

form_changelog::form_changelog(pkgwindow *handler, QWidget *parent) :
        form_handler(handler, parent),
        ui(new Ui::form_changelog)
{
        ui->setupUi(this);

        changelog_reload();

        connect(ui->btn_changelog_add, &QPushButton::clicked, this, &form_changelog::changelog_add);
        connect(ui->btn_changelog_remove, &QPushButton::clicked, this, &form_changelog::changelog_remove);
        connect(ui->btn_changelog_save, &QPushButton::clicked, this, &form_changelog::changelog_save);

        connect(ui->lsw_changelog, &QListWidget::currentRowChanged, this, &form_changelog::changelog_change);
}

form_changelog::~form_changelog()
{
        delete ui;
}


void form_changelog::changelog_add()
{
        changelog *cl = new changelog(0, m_handler->m_pkg, "", "New changelog", "", "");
        m_handler->m_db->cl_insert(cl);
        changelog_reload();
}

void form_changelog::changelog_remove()
{
        changelog *current = m_changelog->at(ui->lsw_changelog->currentRow());
        m_handler->m_db->cl_remove(current->m_id);
        changelog_reload();
}

void form_changelog::changelog_save()
{
        changelog *cl = new changelog(0, m_handler->m_pkg, ui->ln_cl_version->text(), ui->txt_changelog->toPlainText().toUtf8(), ui->ln_status->text(), ui->cb_urgency->currentText()); //FIXME: "0" AS INVALID ID???
        changelog *current = m_changelog->at(ui->lsw_changelog->currentRow());
        m_handler->m_db->cl_update(current->m_id, cl);
        changelog_reload();
}

void form_changelog::changelog_reload()
{
        m_changelog = m_handler->m_db->cl_fetch(m_handler->m_pkg);
        ui->lsw_changelog->clear();
        if(m_changelog == nullptr || m_changelog->isEmpty()) {
                return;
        }
        ui->lsw_changelog->addItems(m_changelog->titles());
}

void form_changelog::changelog_change(int row)
{
        if(row < 0 || row >= m_changelog->size())
                return;
        changelog* selected = m_changelog->at(row);
        ui->txt_changelog->setText(selected->m_text);
        ui->ln_status->setText(selected->m_status);
        ui->cb_urgency->setCurrentText(selected->m_urgency);
        ui->ln_cl_version->setText(selected->m_version);
}

void form_changelog::generate()
{
        m_handler->m_dpkg->changelogs(m_changelog);
}
