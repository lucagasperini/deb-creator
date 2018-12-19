#ifndef FORM_BUILD_H
#define FORM_BUILD_H

#include "form_handler.h"

#include <QFileSystemModel>


namespace Ui
{
class form_build;
}

class form_build : public form_handler
{
        Q_OBJECT

public:
        explicit form_build(pkgwindow *handler, QWidget *parent = nullptr);
        ~form_build();
public slots:
        void compile_import_local();
        void compile_import_remote();
        void build();
        void build_add();
        void build_remove();
        void build_reload();
        void build_select(int row);
        void build_browse_app();
        void build_browse_dir();
        void install_dependencies();
        void generate();
private:
        Ui::form_build *ui;
        QFileSystemModel *m_model_compile;
        multiprocess *m_process;
        list_build_step* m_build_db;
};

#endif // FORM_BUILD_H
