#include "oclanguagemodel.h"

const int OcLanguageModel::NameRole = Qt::UserRole + 1;
const int OcLanguageModel::ValueRole = Qt::UserRole + 2;

OcLanguageModel::OcLanguageModel(QObject *parent) :
    QAbstractListModel(parent)
{
    init();
}


QHash<int, QByteArray> OcLanguageModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(NameRole, QByteArray("name"));
    roles.insert(ValueRole, QByteArray("value"));
    return roles;
}


int OcLanguageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_langs.size();
}


QVariant OcLanguageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() > rowCount()-1)
        return QVariant();

    QString value = m_langs.at(index.row());
    switch(role) {
    case NameRole:
        return QVariant::fromValue(m_langCode[value]);
    case ValueRole:
        return QVariant::fromValue(value);
    default:
        return QVariant();
    }
}


QModelIndex OcLanguageModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
             return QModelIndex();

    return createIndex(row, column);
}


void OcLanguageModel::init()
{
    m_langs << "C" << "en_GB" << "da" << "de" << "el" << "es" << "fi" << "fr" << "gl" << "it" << "nl_NL" << "ru" << "zh_CN";
    m_langCode["C"] = tr("System default");
    m_langCode["en_GB"] = tr("English");
    m_langCode["da"] = tr("Danish");
    m_langCode["de"] = tr("German");
    m_langCode["el"] = tr("Greek");
    m_langCode["es"] = tr("Spanish");
    m_langCode["fi"] = tr("Finnish");
    m_langCode["fr"] = tr("French");
    m_langCode["gl"] = tr("Galician");
    m_langCode["it"] = tr("Italian");
    m_langCode["nl_NL"] = tr("Dutch (Netherlands)");
    m_langCode["ru"] = tr("Russian");
    m_langCode["zh_CN"] = tr("Chinese (China)");
}


QString OcLanguageModel::getLanguageName(const QString &langCode)
{
    return m_langCode[langCode];
}
