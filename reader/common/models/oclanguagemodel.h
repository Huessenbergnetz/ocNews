#ifndef OCLANGUAGEMODEL_H
#define OCLANGUAGEMODEL_H

#include <QAbstractListModel>
#include <QHash>

class OcLanguageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit OcLanguageModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;

    static const int ValueRole;
    static const int NameRole;

    QString getLanguageName(const QString &langCode);

signals:

public slots:

private:
    QList<QString> m_langs;
    QHash<QString, QString> m_langCode;
    void init();


};

#endif // OCLANGUAGEMODEL_H
