#ifndef DEFINE_H
#define DEFINE_H

#ifdef QT_DEBUG
#include <QDebug>
#include <QtSql/QSqlError>
#endif

#include <QObject>
#include <QDir>

#define QSL(t) QStringLiteral(t)

#define SYS_PROGRAM_DIR         QSL("/usr/bin/")

#define DEB_CREATOR_LOCAL       QDir::homePath() + QSL("/.local/share/deb-creator/")
#define DEB_CREATOR_DB          QDir::homePath() + QSL("/.local/share/deb-creator/deb-creator.db")
#define DEB_CREATOR_TMP         QDir::tempPath() + QSL("/deb-creator/")
#define DEB_CREATOR_SRC         QDir::tempPath() + QSL("/deb-creator/src/")
#define DEB_CREATOR_PKG         QDir::tempPath() + QSL("/deb-creator/pkg/")

#define PKG_NAME                QSL(":name")
#define PKG_MAINTAINER          QSL(":maintainer")
#define PKG_UPLOADER            QSL(":uploader")
#define PKG_VERSION             QSL(":version")
#define PKG_HOMEPAGE            QSL(":homepage")
#define PKG_SOURCE              QSL(":source")
#define PKG_SIZE                QSL(":size")
#define PKG_ARCH                QSL(":arch")
#define PKG_DEPEND              QSL(":depend")
#define PKG_BUILD_DEP           QSL(":build_dep")
#define PKG_REPLACE             QSL(":replace")
#define PKG_SECTION             QSL(":section")
#define PKG_DESC_TITLE          QSL(":title")
#define PKG_DESC_BODY           QSL(":body")

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
                                "build_dep TEXT,"               \
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
                                "build_dep,"                    \
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
                                ":build_dep,"                   \
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
                                "build_dep=:build_dep,"         \
                                "replace=:replace,"             \
                                "section=:section,"             \
                                "title=:title,"                 \
                                "body=:body"                    \
                                " WHERE name=:name;")


#endif // DEFINE_H
