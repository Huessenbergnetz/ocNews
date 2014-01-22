#ifndef GLOBALS_H
#define GLOBALS_H

#define VERSION_STRING "1.5.1"
#define VERSION 151
#define VER_MAJ 1
#define VER_MIN 5
#define VER_PAT 1

#if defined(MEEGO_EDITION_HARMATTAN)
#define SETTINGS_PATH "/private"
#define BASE_PATH "/.local/share/data/ocNews"
#define GLOBAL_CERTS_PATH "/etc/ssl/certs"
#else
#define SETTINGS_PATH "/.config/harbour-ocnews"
#define BASE_PATH "/.local/share/data/harbour-ocnews"
#define GLOBAL_CERTS_PATH "/etc/ssl/certs"
#endif


#endif // GLOBALS_H
