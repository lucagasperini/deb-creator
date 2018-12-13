#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QFileInfo>

mainwindow::mainwindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::mainwindow),
        ui_about(new about)
{
        m_db = new database;
        filesystem::debcreator_directory();

        ui->setupUi(this);


        connect(ui->btn_reload, &QPushButton::clicked, this, &mainwindow::reload);
        connect(ui->btn_add, &QPushButton::clicked, this, &mainwindow::package_add);
        connect(ui->btn_remove, &QPushButton::clicked, this, &mainwindow::package_remove);

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
        ui->lsw_welcome->addItems(m_pkglist->names());
}


void mainwindow::open(const QString &name)
{
        if(name.isEmpty())
                return;

        QString buffer;
        for(int i = 0; i < ui->tabWidget->count(); i++) {
                buffer = ui->tabWidget->tabText(i).remove('&');
                if(buffer == name) {
                        ui->tabWidget->setCurrentIndex(i);
                        return;
                }
        }

        package* pkg = m_pkglist->byname(name);
        if(pkg == nullptr) {
                pkg = new package();
                pkg->m_name = name;
        }

        pkgwindow *w = new pkgwindow(*pkg);
        ui->tabWidget->addTab(w, name);
        ui->tabWidget->setCurrentWidget(w);
}

void mainwindow::close(int index)
{
        ui->tabWidget->removeTab(index);
}

void mainwindow::package_add()
{
        open(QSL("Empty"));
}

void mainwindow::package_remove()
{
        QString name = ui->lsw_welcome->currentItem()->text();
        m_db->pkg_remove(m_pkglist->byname(name)->m_id);
        reload();
}

void mainwindow::control_load()
{
        QString filename = QFileDialog::getOpenFileName(this, QSL("Select a valid control file"), QDir::homePath());
        if(filename.isEmpty()) {
                QMessageBox::warning(this, QSL("Control File"), QSL("File name cannot be empty!"));
                return;
        }
        package *tmp = new package(filesystem::file_read(filename));
        if(tmp == nullptr || tmp->is_empty()) {
                QMessageBox::warning(this, QSL("Control File"), filename + QSL(" control file is not valid!"));
                return;
        }
        pkgwindow *w = new pkgwindow(*tmp);
        ui->tabWidget->addTab(w, tmp->m_name);
        ui->tabWidget->setCurrentWidget(w);
}
