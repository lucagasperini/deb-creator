#include "depend.h"
#include "ui_depend.h"
#include <lib/define.h>

depend::depend(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::depend),
        m_apt(new apt),
        ui_add(new depend_add)
{
        ui->setupUi(this);

        connect(ui->btn_search, &QPushButton::clicked, this, &depend::search);
        connect(ui->btn_add, &QPushButton::clicked, this, &depend::add);
        connect(ui->btn_remove, &QPushButton::clicked, this, &depend::remove);
}

depend::~depend()
{
        delete ui;
}

void depend::setup(const QString &deps)
{
        if(deps.isEmpty()) {
                ui->list_dependency->clear();
                return;
        }

        QString str = deps;
        QStringList buffer = str.split(',');
        QStringList trimmed;

        for(int i = 0; i < buffer.size(); i++)
                trimmed << buffer.at(i).trimmed();
        ui->list_dependency->clear();
        ui->list_dependency->addItems(trimmed);
}

QString depend::ok()
{
        QString offset = "";
        int size = ui->list_dependency->count() - 1;

        if(size < 0)
                return offset;

        for(int i = 0; i < size; i++)
                offset.append(ui->list_dependency->item(i)->text() +  ",");
        offset.append(ui->list_dependency->item(size)->text());

        return offset;
}

int depend::contains(const QString &text)
{
        int row = -1;
        for(int i = 0; i < ui->list_dependency->count(); i++)
                if(ui->list_dependency->item(i)->text() == text)
                        row = i;

        return row;
}

void depend::search()
{
        QString search_text = ui->ln_search->text();
        ui->list_search->clear();
        m_search = m_apt->search(search_text);
        if(m_search == nullptr)
                return;

        for(int i = 0; i < m_search->size(); i++)
                ui->list_search->addItem(m_search->at(i)->format(PKG_NAME + " (" + PKG_VERSION + ")"));
}

void depend::add()
{
        package* current = m_search->at(ui->list_search->currentRow());
        ui_add->setup(current->m_name, current->m_version);
        if(ui_add->exec() == QDialog::Rejected)
                return;

        QString version = ui_add->ok();
        QString str;

        if(version.isEmpty())
                str = current->m_name;
        else
                str = current->m_name + " (" + version + ")";

        if(contains(str) == -1)
                ui->list_dependency->addItem(str);
}

void depend::remove()
{
        QListWidgetItem *item = ui->list_dependency->currentItem();
        ui->list_dependency->removeItemWidget(item);
        delete item;
}
