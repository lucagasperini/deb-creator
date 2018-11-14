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
        static void cp(const QString &src, const QString &dest);
        /**
         * @brief file_write write data in selected filename
         * @param filename target file
         * @param data binary data to write
         * @return if cannot write in target file it will return false, otherwise true
         */
        static bool file_write(const QString &filename, const QByteArray &data);
        /**
         * @brief file_read read binary data from target file
         * @param filename file where data is stored
         * @return data from target file
         */
        static QByteArray file_read(const QString &filename);
        static void debcreator_directory();
};

#endif // FILESYSTEM_H
