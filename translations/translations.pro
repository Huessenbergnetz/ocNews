TEMPLATE = subdirs

SUBDIRS += common

!isEmpty(MEEGO_VERSION_MAJOR) {
SUBDIRS += harmattan
} else {
SUBDIRS += sailfishos
}
