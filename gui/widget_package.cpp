#include "widget_package.h"
#include <define.h>

widget_package::widget_package(QWidget *parent) :
        QWidget(parent)
{
        setupUi();
}

widget_package::~widget_package()
{
}

void widget_package::load(const package &pkg)
{
        ln_name->setText(pkg.m_name);
        ln_version->setText(pkg.m_version);
        cb_arch->setCurrentText(package::architecture_name(pkg.m_arch));
        ln_dependancies->setText(pkg.m_depends);
        ln_maintainer->setText(pkg.m_maintainer);
        ln_descriptiontitle->setText(pkg.m_desc_title);
        txt_description->setText(pkg.m_desc_body);
        ln_homepage->setText(pkg.m_homepage);
        ln_replace->setText(pkg.m_replace);
        ln_section->setText(pkg.m_section);
        ln_source->setText(pkg.m_source);
        ln_uploaders->setText(pkg.m_uploaders);

}

package* widget_package::save()
{
        package *pkg = new package;

        pkg->m_name = ln_name->text();
        pkg->m_dir = DEB_CREATOR_PKG + pkg->m_name;
        pkg->m_version = ln_version->text();
        pkg->m_arch = package::architecture_value(cb_arch->currentText());
        pkg->m_depends = ln_dependancies->text();
        pkg->m_maintainer = ln_maintainer->text();
        pkg->m_desc_title = ln_descriptiontitle->text();
        pkg->m_desc_body = txt_description->toPlainText();
        pkg->m_homepage = ln_homepage->text();
        pkg->m_replace = ln_replace->text();
        pkg->m_section = ln_section->text();
        pkg->m_source = ln_source->text();
        pkg->m_uploaders = ln_uploaders->text();

        return pkg;
}

void widget_package::setupUi()
{
        if (objectName().isEmpty())
                setObjectName(QStringLiteral("widget_package"));
        resize(800, 600);
        formLayout = new QFormLayout(this);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        lbl_name = new QLabel(this);
        lbl_name->setObjectName(QStringLiteral("lbl_name"));
        lbl_name->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, lbl_name);

        ln_name = new QLineEdit(this);
        ln_name->setObjectName(QStringLiteral("ln_name"));

        formLayout->setWidget(0, QFormLayout::FieldRole, ln_name);

        lbl_version = new QLabel(this);
        lbl_version->setObjectName(QStringLiteral("lbl_version"));
        lbl_version->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::LabelRole, lbl_version);

        ln_version = new QLineEdit(this);
        ln_version->setObjectName(QStringLiteral("ln_version"));

        formLayout->setWidget(1, QFormLayout::FieldRole, ln_version);

        lbl_maintainer = new QLabel(this);
        lbl_maintainer->setObjectName(QStringLiteral("lbl_maintainer"));
        lbl_maintainer->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::LabelRole, lbl_maintainer);

        ln_maintainer = new QLineEdit(this);
        ln_maintainer->setObjectName(QStringLiteral("ln_maintainer"));

        formLayout->setWidget(2, QFormLayout::FieldRole, ln_maintainer);

        lbl_uploaders = new QLabel(this);
        lbl_uploaders->setObjectName(QStringLiteral("lbl_uploaders"));
        lbl_uploaders->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(3, QFormLayout::LabelRole, lbl_uploaders);

        ln_uploaders = new QLineEdit(this);
        ln_uploaders->setObjectName(QStringLiteral("ln_uploaders"));

        formLayout->setWidget(3, QFormLayout::FieldRole, ln_uploaders);

        lbl_homepage = new QLabel(this);
        lbl_homepage->setObjectName(QStringLiteral("lbl_homepage"));
        lbl_homepage->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(4, QFormLayout::LabelRole, lbl_homepage);

        ln_homepage = new QLineEdit(this);
        ln_homepage->setObjectName(QStringLiteral("ln_homepage"));

        formLayout->setWidget(4, QFormLayout::FieldRole, ln_homepage);

        lbl_source = new QLabel(this);
        lbl_source->setObjectName(QStringLiteral("lbl_source"));
        lbl_source->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(5, QFormLayout::LabelRole, lbl_source);

        ln_source = new QLineEdit(this);
        ln_source->setObjectName(QStringLiteral("ln_source"));

        formLayout->setWidget(5, QFormLayout::FieldRole, ln_source);

        lbl_architecture = new QLabel(this);
        lbl_architecture->setObjectName(QStringLiteral("lbl_architecture"));
        lbl_architecture->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(6, QFormLayout::LabelRole, lbl_architecture);

        cb_arch = new QComboBox(this);
        cb_arch->setObjectName(QStringLiteral("cb_arch"));
        cb_arch->setEditable(true);

        formLayout->setWidget(6, QFormLayout::FieldRole, cb_arch);

        lbl_dependency = new QLabel(this);
        lbl_dependency->setObjectName(QStringLiteral("lbl_dependency"));

        formLayout->setWidget(7, QFormLayout::LabelRole, lbl_dependency);

        ln_dependancies = new QLineEdit(this);
        ln_dependancies->setObjectName(QStringLiteral("ln_dependancies"));
        ln_dependancies->setEnabled(true);

        formLayout->setWidget(7, QFormLayout::FieldRole, ln_dependancies);

        lbl_replace = new QLabel(this);
        lbl_replace->setObjectName(QStringLiteral("lbl_replace"));
        lbl_replace->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(8, QFormLayout::LabelRole, lbl_replace);

        ln_replace = new QLineEdit(this);
        ln_replace->setObjectName(QStringLiteral("ln_replace"));

        formLayout->setWidget(8, QFormLayout::FieldRole, ln_replace);

        lbl_section = new QLabel(this);
        lbl_section->setObjectName(QStringLiteral("lbl_section"));
        lbl_section->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(9, QFormLayout::LabelRole, lbl_section);

        ln_section = new QLineEdit(this);
        ln_section->setObjectName(QStringLiteral("ln_section"));

        formLayout->setWidget(9, QFormLayout::FieldRole, ln_section);

        lbl_descriptiontitle = new QLabel(this);
        lbl_descriptiontitle->setObjectName(QStringLiteral("lbl_descriptiontitle"));
        lbl_descriptiontitle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(10, QFormLayout::LabelRole, lbl_descriptiontitle);

        ln_descriptiontitle = new QLineEdit(this);
        ln_descriptiontitle->setObjectName(QStringLiteral("ln_descriptiontitle"));

        formLayout->setWidget(10, QFormLayout::FieldRole, ln_descriptiontitle);

        lbl_description = new QLabel(this);
        lbl_description->setObjectName(QStringLiteral("lbl_description"));
        lbl_description->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(11, QFormLayout::LabelRole, lbl_description);

        txt_description = new QTextEdit(this);
        txt_description->setObjectName(QStringLiteral("txt_description"));

        formLayout->setWidget(11, QFormLayout::FieldRole, txt_description);

        retranslateUi();

        QMetaObject::connectSlotsByName(this);
} // setupUi

void widget_package::retranslateUi()
{
        setWindowTitle(QApplication::translate("widget_package", "Form", Q_NULLPTR));
        lbl_name->setText(QApplication::translate("widget_package", "Name", Q_NULLPTR));
        ln_name->setText(QString());
        lbl_version->setText(QApplication::translate("widget_package", "Version", Q_NULLPTR));
        ln_version->setText(QString());
        lbl_maintainer->setText(QApplication::translate("widget_package", "Maintainer", Q_NULLPTR));
        ln_maintainer->setText(QString());
        lbl_uploaders->setText(QApplication::translate("widget_package", "Uploaders", Q_NULLPTR));
        lbl_homepage->setText(QApplication::translate("widget_package", "Homepage", Q_NULLPTR));
        lbl_source->setText(QApplication::translate("widget_package", "Source", Q_NULLPTR));
        lbl_architecture->setText(QApplication::translate("widget_package", "Architecture", Q_NULLPTR));
        cb_arch->clear();
        cb_arch->insertItems(0, QStringList()
                             << QApplication::translate("widget_package", "all", Q_NULLPTR)
                             << QApplication::translate("widget_package", "amd64", Q_NULLPTR)
                             << QApplication::translate("widget_package", "i386", Q_NULLPTR)
                            );
        lbl_dependency->setText(QApplication::translate("widget_package", "Dependency", Q_NULLPTR));
        lbl_replace->setText(QApplication::translate("widget_package", "Replace", Q_NULLPTR));
        lbl_section->setText(QApplication::translate("widget_package", "Section", Q_NULLPTR));
        lbl_descriptiontitle->setText(QApplication::translate("widget_package", "Description:  title", Q_NULLPTR));
        lbl_description->setText(QApplication::translate("widget_package", "body", Q_NULLPTR));
} // retranslateUi
