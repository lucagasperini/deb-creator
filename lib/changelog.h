#ifndef CHANGELOG_H
#define CHANGELOG_H

#include "package.h"

#include <QObject>

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
        bool generate(const QString &text, const QString &status, const QString &urgency);
        /**
         * @brief fetch_changelog fetch previous changelog
         * @return list of changelogs
         */
        QStringList fetch();
        void save();

        QByteArray m_text;
        const package* m_pkg;
};

#endif // CHANGELOG_H
