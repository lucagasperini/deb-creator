#ifndef PKGWINDOW_H
#define PKGWINDOW_H

#include <QMainWindow>

#include <debcreator.h>

namespace Ui
{
class pkgwindow;
}

class pkgwindow : public QMainWindow
{
        Q_OBJECT

public:
        explicit pkgwindow(const package &pkg, QWidget *parent = nullptr);
        ~pkgwindow();
        package *m_pkg;
        database *m_db;
        dpkg *m_dpkg;
public slots:
        void output_append(const QString &text);
        void output_clear();
        void package_generate();

private:
        Ui::pkgwindow *ui;
};

#endif // PKGWINDOW_H
