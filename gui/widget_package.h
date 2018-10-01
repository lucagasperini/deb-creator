#ifndef WIDGET_PACKAGE_H
#define WIDGET_PACKAGE_H

#include "package.h"

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

class widget_package : public QWidget
{
        Q_OBJECT

public:
        explicit widget_package(QWidget *parent = 0);
        ~widget_package();

        void load(const package &pkg);

        package *save();

        void setupUi();
        void retranslateUi();

        QFormLayout *formLayout;
        QLabel *lbl_name;
        QLineEdit *ln_name;
        QLabel *lbl_version;
        QLineEdit *ln_version;
        QLabel *lbl_maintainer;
        QLineEdit *ln_maintainer;
        QLabel *lbl_uploaders;
        QLineEdit *ln_uploaders;
        QLabel *lbl_homepage;
        QLineEdit *ln_homepage;
        QLabel *lbl_source;
        QLineEdit *ln_source;
        QLabel *lbl_architecture;
        QComboBox *cb_arch;
        QLabel *lbl_dependency;
        QLineEdit *ln_dependancies;
        QLabel *lbl_replace;
        QLineEdit *ln_replace;
        QLabel *lbl_section;
        QLineEdit *ln_section;
        QLabel *lbl_descriptiontitle;
        QLineEdit *ln_descriptiontitle;
        QLabel *lbl_description;
        QTextEdit *txt_description;

};

#endif // WIDGET_PACKAGE_H
