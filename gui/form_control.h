#ifndef FORM_CONTROL_H
#define FORM_CONTROL_H

#include "form_handler.h"
#include "depend.h"

namespace Ui
{
class form_control;
}

class form_control : public form_handler
{
        Q_OBJECT

public:
        explicit form_control(pkgwindow *handler, QWidget *parent = nullptr);
        ~form_control();
public slots:
        void save();
        void depend_show();
        void depend_build_show();
        void control_generate();
        void generate();

private:
        Ui::form_control *ui;
        depend* ui_dep;
};

#endif // FORM_CONTROL_H
