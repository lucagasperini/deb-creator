#ifndef CHANGELOG_H
#define CHANGELOG_H

#include "package.h"

#include <QObject>


class changelog : public QObject
{
        Q_OBJECT
public:
        explicit changelog(QObject *parent = nullptr);
        explicit changelog(int id, const package* pkg, const QString &version, const QByteArray& text, const QString &status, const QString &urgency, QObject *parent = nullptr);
        /**
         * @brief generate format the changelog text
         * @return the formatted text for changelogs
         */
        QByteArray generate();
        /**
         * @brief clear remove all changelog_t from memory
         */
        void clear();
        /**
         * @brief title get the title of changelog
         * @return title of changelog
         */
        QString title();

        int m_id;
        const package* m_pkg;
        QString m_version;
        QByteArray m_text;
        QString m_status;
        QString m_urgency;
};

#endif // CHANGELOG_H
