 #!/bin/bash
 
STARTDIR=$PWD

cd engine/l10n

for LANG in el en en_GB en_US da de es fr fi gl it nl_NL ru zh_CN
do
lrelease ocnewsengine_$LANG.ts
done

cd ../../account/harmattan/l10n
# lrelease -markuntranslated '' -idbased ocnewsaccount.ts

for LANG in el en en_GB en_US da de es fr fi gl it nl_NL ru zh_CN
do
lrelease -markuntranslated '' -idbased ocnewsaccount_$LANG.ts
done

cd ../../../reader/harmattan/l10n

for LANG in el en en_GB en_US da de es fr fi gl it nl_NL ru zh_CN
do
lrelease ocnewsreader_$LANG.ts
done

cd ../../sailfishos/l10n

for LANG in el en en_GB en_US da de es fr fi gl it nl_NL ru zh_CN
do
lrelease-qt5 ocnewsreader_$LANG.ts
done
