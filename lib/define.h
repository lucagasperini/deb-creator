#ifndef DEFINE_H
#define DEFINE_H

#define DB_PACKAGE_TABLE QStringLiteral("package")
#define DB_PACKAGE_CREATE QStringLiteral("CREATE TABLE package("\
                                "name TEXT PRIMARY KEY,"        \
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

#define DB_PACKAGE_INSERT QStringLiteral("INSERT INTO package ("\
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

#define DB_PACKAGE_UPDATE QStringLiteral("UPDATE package SET "  \
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
