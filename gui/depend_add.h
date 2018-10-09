#ifndef DEPEND_ADD_H
#define DEPEND_ADD_H

#include <QDialog>

namespace Ui
{
class depend_add;
}

class depend_add : public QDialog
{
        Q_OBJECT

public:
        explicit depend_add(QWidget *parent = 0);
        ~depend_add();
        void setup(const QString &name, const QString &version);
        QString ok();
public slots:
        void radio_current(bool status);
        void radio_manual(bool status);

private:
        Ui::depend_add *ui;
};

#endif // DEPEND_ADD_H
