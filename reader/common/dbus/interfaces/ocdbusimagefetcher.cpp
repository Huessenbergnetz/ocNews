#include "ocdbusimagefetcher.h"

OcDBusImageFetcher::OcDBusImageFetcher(QObject *parent) :
    QObject(parent)
{
    imageFetcher = new de::buschmann23::ocNewsEngine::ImageFetcher("de.buschmann23.ocNewsEngine", "/ImageFetcher", QDBusConnection::sessionBus(), this);
    connect(imageFetcher, SIGNAL(startedFetchingImages(int)), this, SLOT(setTotal(int)));
    connect(imageFetcher, SIGNAL(finishedFetchingImages(int)), this, SLOT(setTotal(int)));
    connect(imageFetcher, SIGNAL(fetchingImages(int)), this, SLOT(setCurrent(int)));

    m_total = imageFetcher->isFetchImagesRunning();
    m_current = 0;
}

int OcDBusImageFetcher::total() const
{
    return m_total;
}


void OcDBusImageFetcher::setTotal(const int &nTotal)
{
    if (nTotal != m_total) {
        m_total = nTotal;
        emit totalChanged(total());
    }
}


int OcDBusImageFetcher::current() const
{
    return m_current;
}


void OcDBusImageFetcher::setCurrent(const int &nCurrent)
{
    if (nCurrent != m_current) {
        m_current = nCurrent;
        emit currentChanged(current());
    }
}
