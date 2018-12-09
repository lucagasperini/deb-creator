#ifndef FORM_HANDLER_H
#define FORM_HANDLER_H

#include "pkgwindow.h"

#include <QWidget>

class form_handler : public QWidget
{
        Q_OBJECT
public:
        explicit form_handler(pkgwindow *handler, QWidget *parent = nullptr);
public slots:
        virtual void generate() = 0;
protected:
        pkgwindow *m_handler;
};

#endif // FORM_HANDLER_H
