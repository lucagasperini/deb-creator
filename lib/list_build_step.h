#ifndef LIST_BUILD_STEP_H
#define LIST_BUILD_STEP_H

#include "build_step.h"

#include <QObject>

class list_build_step : public QObject, public QList<build_step*>
{
        Q_OBJECT
public:
        explicit list_build_step(QObject *parent = nullptr);

        /**
         * @brief titles get all titles from build_steps
         * @return string list with titles
         */
        QStringList titles();
        /**
         * @brief find a build_step from this list
         * @param p the build_step to find
         * @return pointer of build_step into this list
         */
        build_step* find(const build_step* p);
};

#endif // LIST_BUILD_STEP_H
