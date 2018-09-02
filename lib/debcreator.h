#ifndef DEBCREATOR_H
#define DEBCREATOR_H

#include <QObject>

class debcreator : public QObject
{
        Q_OBJECT
public:
        explicit debcreator(QObject *parent = nullptr);
        QString control();
        QString package(const QString& control);

        QString m_package;
        QString m_version;
        QString m_arch;
        QString m_depends;
        QString m_maintainer;
        QString m_desc_title;
        QString m_desc_body;

        QString m_control;
        QString m_dir;
        QString m_outputfile;
};

#endif // DEBCREATOR_H
