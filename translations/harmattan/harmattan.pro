TEMPLATE = subdirs

#system ("rm *.qm")
#system ("lrelease -markuntranslated '' -idbased *.ts")

qmfiles.files = ocnewsaccount*.qm
qmfiles.path = /usr/share/l10n/meegotouch
INSTALLS += qmfiles

OTHER_FILES += *.ts
