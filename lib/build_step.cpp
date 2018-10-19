#include "build_step.h"

build_step::build_step(QObject *parent) : QObject(parent)
{

}

build_step::build_step(const build_step &step)
{
        program = step.program;
        argument = step.argument;
        directory = step.directory;
}
