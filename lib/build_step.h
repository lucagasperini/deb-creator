#ifndef BUILD_STEP_H
#define BUILD_STEP_H

#include <QObject>

class build_step : public QObject
{
        Q_OBJECT
public:
        explicit build_step(QObject *parent = nullptr);
        explicit build_step(int id, int pkg, QString app, QString arg, QString dir, QObject *parent = nullptr);
        explicit build_step(const build_step &step);

        int m_id;
        int m_pkg;
        QString m_app;
        QString m_arg;
        QString m_dir;
};

#endif // BUILD_STEP_H
