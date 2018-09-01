#ifndef DEBCREATOR_H
#define DEBCREATOR_H

#include <QObject>

class DebCreator : public QObject
{
    Q_OBJECT
public:
    explicit DebCreator(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DEBCREATOR_H