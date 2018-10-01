#include "depend.h"
#include "ui_depend.h"

depend::depend(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::depend),
        m_apt(new apt)
{
        ui->setupUi(this);

        connect(ui->btn_search, &QPushButton::clicked, this, &depend::search);
}

depend::~depend()
{
        delete ui;
}

void depend::search()
{
        QList<package*>* buffer = m_apt->search(ui->ln_search->text());
        for(int i = 0; i < buffer->size(); i++)
                ui->list_search->addItem(buffer->at(i)->m_name);
}
