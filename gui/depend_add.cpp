#include "depend_add.h"
#include "ui_depend_add.h"

depend_add::depend_add(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::depend_add)
{
        ui->setupUi(this);

        connect(ui->rad_current, &QRadioButton::toggled, this, &depend_add::radio_current);
        connect(ui->rad_manual, &QRadioButton::toggled, this, &depend_add::radio_manual);
}

depend_add::~depend_add()
{
        delete ui;
}

void depend_add::setup(const QString &name, const QString &version)
{
        ui->lbl_add->setText(QStringLiteral("You are adding as dependency the following package: ") + name);
        ui->ln_current->setText(version);
        ui->rad_empty->setChecked(true);
}

QString depend_add::ok()
{
        if(ui->rad_manual->isChecked())
                return ui->ln_manual->text();
        else if (ui->rad_current->isChecked())
                return ui->ln_current->text();
        else
                return "";
}

void depend_add::radio_current(bool status)
{
        ui->ln_current->setEnabled(status);
}

void depend_add::radio_manual(bool status)
{
        ui->ln_manual->setEnabled(status);
}
