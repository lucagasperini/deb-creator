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

        int contains(const QString &text);
        QString ok();

private slots:
        void search();
        void add();
        void remove();
private:
        Ui::depend *ui;
        apt* m_apt;
        QStringList m_deps;
};

#endif // DEPEND_H
