#ifndef DEFINE_H
#define DEFINE_H

#ifdef QT_DEBUG
#include <QDebug>
#include <QtSql/QSqlError>
#endif

#include <QObject>
#include <QDir>

#define QSL(t) QStringLiteral(t)

#define DEB_CREATOR_LOCAL       QDir::homePath() + QSL("/.local/share/deb-creator/")
#define DEB_CREATOR_DB          QDir::homePath() + QSL("/.local/share/deb-creator/deb-creator.db")
#define DEB_CREATOR_TMP         QDir::tempPath() + QSL("/deb-creator/")
#define DEB_CREATOR_SRC         QDir::tempPath() + QSL("/deb-creator/src/")

#define DB_PACKAGE_TABLE QSL("package")
#define DB_PACKAGE_CREATE QSL("CREATE TABLE package("           \
                                "name TEXT PRIMARY KEY,"        \
                                "directory TEXT,"               \
                                "maintainer TEXT,"              \
                                "uploader TEXT,"                \
                                "version TEXT,"                 \
                                "homepage TEXT,"                \
                                "source TEXT,"                  \
                                "arch TEXT,"                    \
                                "depend TEXT,"                  \
                                "replace TEXT,"                 \
                                "section TEXT,"                 \
                                "title TEXT,"                   \
                                "body TEXT"                     \
                                ");")

#define DB_PACKAGE_INSERT QSL("INSERT INTO package ("           \
                                "name,"                         \
                                "directory,"                    \
                                "maintainer,"                   \
                                "uploader,"                     \
                                "version,"                      \
                                "homepage,"                     \
                                "source,"                       \
                                "arch,"                         \
                                "depend,"                       \
                                "replace,"                      \
                                "section,"                      \
                                "title,"                        \
                                "body"                          \
                                ") VALUES ("                    \
                                ":name,"                        \
                                ":directory,"                   \
                                ":maintainer,"                  \
                                ":uploader,"                    \
                                ":version,"                     \
                                ":homepage,"                    \
                                ":source,"                      \
                                ":arch,"                        \
                                ":depend,"                      \
                                ":replace,"                     \
                                ":section,"                     \
                                ":title,"                       \
                                ":body"                         \
                                ");")

#define DB_PACKAGE_UPDATE QSL("UPDATE package SET "             \
                                "maintainer=:maintainer,"       \
                                "directory=:directory,"         \
                                "uploader=:uploader,"           \
                                "version=:version,"             \
                                "homepage=:homepage,"           \
                                "source=:source,"               \
                                "arch=:arch,"                   \
                                "depend=:depend,"               \
                                "replace=:replace,"             \
                                "section=:section,"             \
                                "title=:title,"                 \
                                "body=:body"                    \
                                " WHERE name=:name;")


#endif // DEFINE_H
