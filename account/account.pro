TEMPLATE = subdirs

!isEmpty(MEEGO_VERSION_MAJOR) {
SUBDIRS += harmattan
} else {
SUBDIRS += sailfishos
}