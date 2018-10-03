#ifndef GIT_H
#define GIT_H

#include <QObject>
#include <QProcess>
#include <QUrl>

class git : public QObject
{
        Q_OBJECT
public:
        explicit git(QObject *parent = nullptr);
        QByteArray execute();
        /**
         * @brief git_clone fetch git repo in default directory
         * @param url repo git
         * @return directory of files stored
         */
        QString clone(const QUrl &url, const QString &directory = "");
        /**
         * @brief git_fetch_user fetch user.email and user.name from git config
         * @return the format from git is user.name <user.email>, empty string if git is not installed or git is not configurated
         */
        QString fetch_user();

        QProcess *m_git;

signals:

public slots:
};

#endif // GIT_H
