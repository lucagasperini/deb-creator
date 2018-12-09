#ifndef FORM_CUSTOM_H
#define FORM_CUSTOM_H

#include <QFileSystemModel>

#include "form_handler.h"

namespace Ui
{
class form_custom;
}

class form_custom : public form_handler
{
        Q_OBJECT

public:
        explicit form_custom(pkgwindow* handler, QWidget *parent = nullptr);
        ~form_custom();
public slots:
        void custom_refresh();
        void custom_save();
        void custom_load(const QModelIndex &a);
        void generate();
private:
        Ui::form_custom *ui;
        QFileSystemModel *m_model_custom;
};

#endif // FORM_CUSTOM_H
