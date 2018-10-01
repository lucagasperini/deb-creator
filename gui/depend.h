#ifndef DEPEND_H
#define DEPEND_H

#include <apt.h>

#include <QDialog>

namespace Ui
{
class depend;
}

class depend : public QDialog
{
        Q_OBJECT

public:
        explicit depend(QWidget *parent = 0);
        ~depend();

private slots:
        void search();
        void select(const QString &pkg);
private:
        Ui::depend *ui;
        apt* m_apt;
};

#endif // DEPEND_H
