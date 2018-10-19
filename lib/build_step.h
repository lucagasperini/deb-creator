#ifndef BUILD_STEP_H
#define BUILD_STEP_H

#include <QObject>

class build_step : public QObject
{
        Q_OBJECT
public:
        explicit build_step(QObject *parent = nullptr);
        explicit build_step(const build_step &step);

        QString program;
        QString argument;
        QString directory;
};

#endif // BUILD_STEP_H
