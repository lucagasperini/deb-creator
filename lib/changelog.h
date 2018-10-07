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
         * @brief generate format the changelog text
         * @param text text in the changelog
         * @param status string of status of the changelog
         * @param urgency string of urgency of changelog
         * @return the formatted text for changelogs
         */
        QByteArray generate(const QString &text, const QString &status, const QString &urgency);
        /**
         * @brief fetch fetch previous changelog
         */
        void fetch();
        /**
         * @brief save append text in changelog file
         * @param text text of the changelog
         */
        void save(const QByteArray &text);
        /**
         * @brief clear remove all changelog_t from memory
         */
        void clear();
        /**
         * @brief titles get the list of titles of changelogs stored
         * @return list of titles of changelogs
         */
        QStringList titles();
        /**
         * @brief text get the text of changelog
         * @param i index of changelog
         * @return the text of changelog
         */
        QByteArray text(int i);

        QList<changelog_t*> m_cl;
        const package *m_pkg;
};

#endif // CHANGELOG_H
