#ifndef GLOBALS_H
#define GLOBALS_H

#define VERSION_STRING "1.6.1"
#define VERSION 161
#define VER_MAJ 1
#define VER_MIN 6
#define VER_PAT 1

#if defined(MEEGO_EDITION_HARMATTAN)
#define SETTINGS_PATH "/private"
#define BASE_PATH "/.local/share/data/ocNews"
#define GLOBAL_CERTS_PATH "/etc/ssl/certs"
#define IMAGE_CACHE "/MyDocs/.ocNews/imageCache"
#else
#define SETTINGS_PATH "/.config/harbour-ocnews"
#define BASE_PATH "/.local/share/data/harbour-ocnews"
#define GLOBAL_CERTS_PATH "/etc/ssl/certs"
#define IMAGE_CACHE "/.local/share/data/harbour-ocnews/imageCache"
#endif


#endif // GLOBALS_H
