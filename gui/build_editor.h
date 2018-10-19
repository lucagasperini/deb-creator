#ifndef BUILD_EDITOR_H
#define BUILD_EDITOR_H

#include <build_step.h>

#include <QDialog>

namespace Ui
{
class build_editor;
}

class build_editor : public QDialog
{
        Q_OBJECT

public:
        explicit build_editor(const QList<build_step*>* steps = nullptr, const QString &_working_dir = "", QWidget *parent = 0);
        ~build_editor();

        QString program;
        QString args;
        QString working_dir;
        const QList<build_step*>* m_step;
public slots:
        void ok();
        void browse_program();
        void browse_dir();
        void list_index();
private:
        Ui::build_editor *ui;
};

#endif // BUILD_EDITOR_H
