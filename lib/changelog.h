#ifndef CHANGELOG_H
#define CHANGELOG_H

#include "package.h"

#include <QObject>


class changelog : public QObject
{
        Q_OBJECT
public:
        explicit changelog(QObject *parent = nullptr);
        explicit changelog(const QByteArray& text, QObject *parent = nullptr);
        /**
         * @brief generate format the changelog text
         * @param text text in the changelog
         * @param status string of status of the changelog
         * @param urgency string of urgency of changelog
         * @return the formatted text for changelogs
         */
        static QByteArray generate(const package *pkg, const QString &text, const QString &status, const QString &urgency);
        /**
         * @brief fetch fetch previous changelog
         */
        static QList<changelog*>* fetch(const QString &file);
        /**
         * @brief save append text in changelog file
         * @param text text of the changelog
         */
        static void save(const QString &file, const QByteArray &text);
        /**
         * @brief clear remove all changelog_t from memory
         */
        void clear();
        /**
         * @brief title get the title of changelog
         * @return title of changelog
         */
        QString title();

        QByteArray m_text;
};

#endif // CHANGELOG_H
