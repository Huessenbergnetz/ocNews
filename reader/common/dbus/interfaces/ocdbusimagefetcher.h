#ifndef OCDBUSIMAGEFETCHER_H
#define OCDBUSIMAGEFETCHER_H

#include <QObject>

#include "ocimagefetcherif.h"

class OcDBusImageFetcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int total READ total WRITE setTotal NOTIFY totalChanged)
    Q_PROPERTY(int current READ current WRITE setCurrent NOTIFY currentChanged)
public:
    explicit OcDBusImageFetcher(QObject *parent = 0);

    int total() const;
    int current() const;

public slots:
    void setTotal(const int &nTotal);
    void setCurrent(const int &nCurrent);

signals:
    void totalChanged(const int &nTotal);
    void currentChanged(const int &nCurrent);

private:
    de::buschmann23::ocNewsEngine::ImageFetcher *imageFetcher;

    int m_total;
    int m_current;
};

#endif // OCDBUSIMAGEFETCHER_H
