#include "depend.h"
#include "ui_depend.h"

depend::depend(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::depend),
        m_apt(new apt)
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

QString depend::ok()
{
        QString offset = "";
        int size = m_deps.size() - 1;
        for(int i = 0; i < size; i++)
                offset.append(m_deps.at(i) +  ",");
        offset.append(m_deps.at(size));
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
        if(search_text.isEmpty())
                return;

        ui->list_search->clear();
        m_apt->clear();
        QStringList result = m_apt->search(search_text);
        ui->list_search->addItems(result);
}

void depend::add()
{
        QString text = ui->list_search->currentItem()->text();
        if(contains(text) == -1) {
                m_deps.append(text);
                ui->list_dependency->addItem(text);
        }
}

void depend::remove()
{
        QListWidgetItem *item = ui->list_dependency->currentItem();
        m_deps.removeOne(item->text());
        ui->list_dependency->removeItemWidget(item);
        delete item;
}
