#include "depend.h"
#include "ui_depend.h"

depend::depend(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::depend),
        m_apt(new apt)
{
        ui->setupUi(this);

        connect(ui->btn_search, &QPushButton::clicked, this, &depend::search);
        connect(ui->list_search, &QListWidget::currentTextChanged, this, &depend::select);
}

depend::~depend()
{
        delete ui;
}

void depend::search()
{
        ui->list_search->clear();
        m_apt->clear();
        QStringList result = m_apt->search(ui->ln_search->text());
        ui->list_search->addItems(result);
}

void depend::select(const QString &pkg)
{
        if(pkg.isEmpty())
                return;
        ui->ui_package->load(*m_apt->cache(pkg));
}
