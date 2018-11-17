#ifndef GIT_H
#define GIT_H

#include <QObject>
#include <QProcess>
#include <QUrl>

class git : public QProcess
{
        Q_OBJECT
public:
        explicit git(QObject *parent = nullptr);
        /**
         * @brief exec execute git process with argument and working directory
         * @return output of git process
         */
        QByteArray exec();
        /**
         * @brief git_clone fetch git repo in default directory
         * @param url repo git
         * @param directory is the name of the directory to clone from
         * @param branch is the branch version to clone
         * @return directory of files stored
         */
        QString clone(const QUrl &url, const QString &directory = "", const QString &branch = "");
        /**
         * @brief git_fetch_user fetch user.email and user.name from git config
         * @return the format from git is user.name <user.email>, empty string if git is not installed or git is not configurated
         */
        QString fetch_user();
};

#endif // GIT_H
