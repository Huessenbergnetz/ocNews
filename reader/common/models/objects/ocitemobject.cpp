#include "ocitemobject.h"

OcItemObject::OcItemObject()
{

}


OcItemObject::OcItemObject(const OcItemObject &other)
{

}


OcItemObject::~OcItemObject()
{

}


OcItemObject::OcItemObject(const int &id, const QString &tit, const uint &pub, const QString &encLink, const QString &encMime, const bool &unr, const bool &star, const QString &u, const QString &hash, const QString &exc, const QString &img, const QString &feed, const int &fId):
          itemId(id),
          title(tit),
          pubDate(pub),
          enclosureLink(encLink),
          enclosureMime(encMime),
          unread(unr),
          starred(star),
          url(u),
          guidHash(hash),
          feedName(feed),
          feedId(fId),
          image(img),
          excerpt(exc)
{}
