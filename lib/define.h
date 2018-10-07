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
#define DEB_CREATOR_PKG         QDir::tempPath() + QSL("/deb-creator/pkg/")

#define APT_SEARCH              QSL("apt-cache search ")

#define DB_PACKAGE_TABLE QSL("package")
#define DB_PACKAGE_CREATE QSL("CREATE TABLE package("           \
                                "name TEXT PRIMARY KEY,"        \
                                "maintainer TEXT,"              \
                                "uploader TEXT,"                \
                                "version TEXT,"                 \
                                "homepage TEXT,"                \
                                "source TEXT,"                  \
                                "arch INTEGER,"                 \
                                "depend TEXT,"                  \
                                "replace TEXT,"                 \
                                "section TEXT,"                 \
                                "title TEXT,"                   \
                                "body TEXT"                     \
                                ");")

#define DB_PACKAGE_INSERT QSL("INSERT INTO package ("           \
                                "name,"                         \
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
