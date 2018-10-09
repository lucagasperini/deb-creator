#ifndef DEPEND_H
#define DEPEND_H

#include <apt.h>

#include "depend_add.h"

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

        void setup(const QString &deps);

        int contains(const QString &text);
        QString ok();

private slots:
        void search();
        void add();
        void remove();
private:
        Ui::depend *ui;
        apt* m_apt;
        QList<package*>* m_search;
        depend_add *ui_add;
};

#endif // DEPEND_H
