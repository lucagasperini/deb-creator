#ifndef CHANGELOG_H
#define CHANGELOG_H

#include <QObject>
#include <QString>

class changelog : public QObject
{
        Q_OBJECT
public:
        explicit changelog(QObject *parent = nullptr);
        /**
         * @brief generate generate and store changelog in this class
         * @param text user log in changelog
         * @return if changelog is created correctly return true otherwise false
         */
        QString generate(const QString &text, const QString &status, const QString &urgency);
        /**
         * @brief fetch fetch previous changelog
         * @return list of changelogs
         */
        QStringList fetch(const QString &file);

        QString save();

        QString m_text;
};

#endif // CHANGELOG_H
