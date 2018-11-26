#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QFileInfo>

using namespace std;

mainwindow::mainwindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::mainwindow)
{
        m_db = new database(DEB_CREATOR_DB, "deb-creator-main");
        filesystem::debcreator_directory();

        ui->setupUi(this);


        connect(ui->btn_reload, &QPushButton::clicked, this, &mainwindow::reload);

        connect(ui->a_quit, &QAction::triggered, qApp, &QApplication::quit);
        connect(ui->a_about, &QAction::triggered, ui_about, &about::show);
        connect(ui->a_aboutqt, &QAction::triggered, qApp, &QApplication::aboutQt);
        connect(ui->lsw_welcome, &QListWidget::currentTextChanged, this, &mainwindow::open);
        connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &mainwindow::close);
        connect(ui->a_loadctrl, &QAction::triggered, this, &mainwindow::control_load);
        // connect(ui->a_manual) TODO: Add a manual?


        reload();
}

mainwindow::~mainwindow()
{
        delete ui;
}


void mainwindow::reload()
{
        m_pkglist = m_db->pkg_fetch();
        if(m_pkglist == nullptr)
                return;

        ui->lsw_welcome->clear();
        ui->lsw_welcome->addItems(m_pkglist->values());
}


void mainwindow::open(const QString &pkg)
{
        package *found = m_db->pkg_fetch(m_pkglist->key(pkg));
        pkgwindow *w = new pkgwindow(*found);
        ui->tabWidget->addTab(w, pkg);
}

void mainwindow::close(int index)
{
        ui->tabWidget->removeTab(index);
}

void mainwindow::control_load()
{
        /*        QString filename = QFileDialog::getOpenFileName(this, QSL("Select a valid control file"), QDir::homePath());
                if(filename.isEmpty()) {
                        QMessageBox::warning(this, QSL("Control File"), QSL("File name cannot be empty!"));
                        return;
                }
                package *tmp = new package(filesystem::file_read(filename));
                if(tmp == nullptr || tmp->is_empty()) {
                        ui->txt_output->append(filename + QSL(" control file is not valid!"));
                        return;
                }
                m_pkg->copy(*tmp);
                load();*/
}
