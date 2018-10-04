#ifndef CHANGELOG_H
#define CHANGELOG_H

#include "package.h"

#include <QObject>

struct changelog_t {
        int index;
        QByteArray text;
};

class changelog : public QObject
{
        Q_OBJECT
public:
        explicit changelog(const package *pkg, QObject *parent = nullptr);
        /**
         * @brief changelog generate and store changelog in this class
         * @param text user log in changelog
         * @return if changelog is created correctly return true otherwise false
         */
        QByteArray generate(const QString &text, const QString &status, const QString &urgency);
        /**
         * @brief fetch_changelog fetch previous changelog
         * @return list of changelogs
         */
        void fetch();
        void save(const QByteArray &text);
        void clear();
        QStringList titles();
        QByteArray text(int i);

        QList<changelog_t*> m_cl;
        const package *m_pkg;
};

#endif // CHANGELOG_H
