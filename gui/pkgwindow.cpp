#include "pkgwindow.h"
#include "ui_pkgwindow.h"

#include "form_control.h"
#include "form_build.h"
#include "form_changelog.h"
#include "form_custom.h"

#include <QMessageBox>
#include <QFileDialog>


#define TAB_CONTROL 0
#define TAB_COMPILE 1
#define TAB_CHANGELOG 2
#define TAB_CUSTOM 3

using namespace std;

pkgwindow::pkgwindow(const package &pkg, QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::pkgwindow)
{

        m_pkg = new package(pkg);
        m_db = new database(DEB_CREATOR_DB, m_pkg->m_name);
        m_dpkg = new dpkg(m_pkg->root());
        ui->setupUi(this);

        form_control *control = new form_control(this);
        ui->tabWidget->addTab(control, "control");
        ui->tabWidget->setCurrentWidget(control);

        form_changelog *changelog = new form_changelog(this);
        ui->tabWidget->addTab(changelog, "changelog");
        ui->tabWidget->setCurrentWidget(changelog);

        form_build *build = new form_build(this);
        ui->tabWidget->addTab(build, "build");
        ui->tabWidget->setCurrentWidget(build);

        form_custom *custom = new form_custom(this);
        ui->tabWidget->addTab(custom, "custom");
        ui->tabWidget->setCurrentWidget(custom);

        ui->tabWidget->setCurrentIndex(TAB_CONTROL);

        connect(ui->btn_createpackage, &QPushButton::clicked, control, &form_handler::generate);
        connect(ui->btn_createpackage, &QPushButton::clicked, changelog, &form_handler::generate);
        connect(ui->btn_createpackage, &QPushButton::clicked, build, &form_handler::generate);
        connect(ui->btn_createpackage, &QPushButton::clicked, custom, &form_handler::generate);
        connect(ui->btn_createpackage, &QPushButton::clicked, this, &pkgwindow::package_generate);
        connect(ui->btn_clear, &QPushButton::clicked, this, &pkgwindow::output_clear);
}

pkgwindow::~pkgwindow()
{
        delete ui;
}

void pkgwindow::output_append(const QString &text)
{
#ifdef QT_DEBUG
        qDebug() << text;
#endif
        ui->txt_output->insertPlainText(text);
}

void pkgwindow::output_clear()
{
        ui->txt_output->clear();
}

void pkgwindow::package_generate()
{
        QString outputfile = QFileDialog::getSaveFileName(this, QSL("Select where save package"), QDir::homePath() + "/" + m_pkg->outputfile());

        if(outputfile.isEmpty()) {
                QMessageBox::warning(this, QSL("Creating Package"), QSL("Output file path cannot be empty!"));
                return;
        }

        output_append(m_dpkg->exec(outputfile));
}
