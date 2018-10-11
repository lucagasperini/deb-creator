#include "filesystem.h"
#include "define.h"

filesystem::filesystem(QObject *parent) : QObject(parent)
{

}

qint64 filesystem::size(const QString &_dir)
{
        qint64 sizex = 0;
        QFileInfo str_info(_dir);
        if (!str_info.isDir()) {
                return str_info.size();
        }

        QDir dir(_dir);
        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for (int i = 0; i < list.size(); ++i) {
                QFileInfo fileInfo = list.at(i);
#ifdef QT_DEBUG
                qDebug() << fileInfo.absoluteFilePath();
#endif
                sizex += size(fileInfo.absoluteFilePath());
        }
        return sizex;
}

void filesystem::cp(const QString &src, const QString &dest)
{
        QFileInfo str_info(src);
        if (!str_info.isDir()) {
                QFile::copy(src, dest);
                return;
        }

        QDir dir(src);
        dir.mkdir(dest);
        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        for (int i = 0; i < list.size(); ++i) {
                QFileInfo fileInfo = list.at(i);
#ifdef QT_DEBUG
                qDebug() << fileInfo.absoluteFilePath();
#endif
                cp(fileInfo.absoluteFilePath(), fileInfo.absoluteFilePath().replace(src, dest));
        }
}
