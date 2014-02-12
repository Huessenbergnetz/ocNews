#ifndef GLOBALS_H
#define GLOBALS_H

#define VERSION_STRING "1.7.0"
#define VERSION 170
#define VER_MAJ 1
#define VER_MIN 7
#define VER_PAT 0

#if defined(MEEGO_EDITION_HARMATTAN)
#define SETTINGS_PATH "/private"
#define BASE_PATH "/.local/share/data/ocNews"
#define GLOBAL_CERTS_PATH "/etc/ssl/certs"
#define IMAGE_CACHE "/MyDocs/.ocNews/imageCache"

#define MEDIA_PATH "/MyDocs/ocNews/media"
#define MEDIA_PATH_AUDIO "/MyDocs/ocNews/media/audio"
#define MEDIA_PATH_IMAGE "/MyDocs/ocNews/media/images"
#define MEDIA_PATH_PDF "/MyDocs/ocNews/media/documents"
#define MEDIA_PATH_VIDEO "/MyDocs/ocNews/media/videos"

#define MIME_ICON_GENERAL "icon-m-content-attachment"
#define MIME_ICON_AUDIO "icon-m-content-audio"
#define MIME_ICON_IMAGE "icon-m-content-image"
#define MIME_ICON_PDF "icon-m-content-pdf"
#define MIME_ICON_VIDEO "icon-m-content-videos"

#define SQL_TRUE "\"true\""
#define SQL_FALSE "\"false\""
#else
#define OLD_SETTINGS_PATH "/.config/harbour-ocnews"
#define SETTINGS_PATH "/.config/harbour-ocnews-reader"
#define OLD_BASE_PATH "/.local/share/data/harbour-ocnews"
#define BASE_PATH "/.local/share/harbour-ocnews-reader"
#define GLOBAL_CERTS_PATH "/etc/ssl/certs"
#define IMAGE_CACHE "/.local/share/harbour-ocnews-reader/imageCache"

#define MEDIA_PATH "/Downloads"
#define MEDIA_PATH_AUDIO "/Music/ocNews"
#define MEDIA_PATH_IMAGE "/Pictures/ocNews"
#define MEDIA_PATH_PDF "/Documents/ocNews"
#define MEDIA_PATH_VIDEO "/Videos/ocNews"

#define MIME_ICON_GENERAL "icon-m-attach"
#define MIME_ICON_AUDIO "icon-m-music"
#define MIME_ICON_IMAGE "icon-m-image"
#define MIME_ICON_PDF "icon-m-document"
#define MIME_ICON_VIDEO "icon-m-video"

#define SQL_TRUE "1"
#define SQL_FALSE "0"
#endif


#endif // GLOBALS_H
