#include "oclanguagemodelfilter.h"

OcLanguageModelFilter::OcLanguageModelFilter(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    this->setSourceModel(&m_sourceModel);
    this->setFilterRole(OcLanguageModel::NameRole);
    this->setFilterCaseSensitivity(Qt::CaseInsensitive);
    this->setSortLocaleAware(true);
    this->setSortRole(OcLanguageModel::NameRole);
    this->sort(0);
}


QString OcLanguageModelFilter::search() const
{
    return m_search;
}


void OcLanguageModelFilter::setSearch(const QString &nSearch)
{
    if (nSearch != m_search) {
        m_search = nSearch;
        this->setFilterFixedString(search());
        emit searchChanged(search());
    }
}


QString OcLanguageModelFilter::getLanguageName(const QString &langCode)
{
    return m_sourceModel.getLanguageName(langCode);
}
