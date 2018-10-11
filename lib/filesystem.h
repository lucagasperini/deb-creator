#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QObject>

class filesystem : public QObject
{
        Q_OBJECT
public:
        explicit filesystem(QObject *parent = nullptr);
        /**
         * @brief size calculate the size of directory
         * @param _dir root directory
         * @return integer 64 bit of size in byte
         */
        static qint64 size(const QString &_dir);

signals:

public slots:
};

#endif // FILESYSTEM_H
