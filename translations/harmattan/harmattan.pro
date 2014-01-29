TEMPLATE = subdirs

#system ("rm *.qm")
#system ("lrelease -markuntranslated '' -idbased *.ts")

qmfiles.files = \
    ocnewsaccount.qm \
    ocnewsaccount_de.qm \
    ocnewsaccount_es.qm \
    ocnewsaccount_fr.qm \
    ocnewsaccount_zh_CN.qm

qmfiles.path = /usr/share/l10n/meegotouch
INSTALLS += qmfiles

OTHER_FILES += *.ts
