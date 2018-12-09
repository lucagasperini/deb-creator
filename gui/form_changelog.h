#ifndef FORM_CHANGELOG_H
#define FORM_CHANGELOG_H

#include "form_handler.h"

namespace Ui
{
class form_changelog;
}

class form_changelog : public form_handler
{
        Q_OBJECT

public:
        explicit form_changelog(pkgwindow *handler, QWidget *parent = nullptr);
        ~form_changelog();
public slots:
        void changelog_add();
        void changelog_remove();
        void changelog_save();
        void changelog_reload();
        void changelog_change(int row);
        void generate();
private:
        Ui::form_changelog *ui;
        changelog_list* m_changelog;
};

#endif // FORM_CHANGELOG_H
