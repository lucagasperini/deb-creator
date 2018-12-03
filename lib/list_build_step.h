#ifndef LIST_BUILD_STEP_H
#define LIST_BUILD_STEP_H

#include "build_step.h"

#include <QObject>

class list_build_step : public QObject, public QList<build_step*>
{
        Q_OBJECT
public:
        explicit list_build_step(QObject *parent = nullptr);

        QStringList titles();
        build_step* find(const build_step* p);
};

#endif // LIST_BUILD_STEP_H
