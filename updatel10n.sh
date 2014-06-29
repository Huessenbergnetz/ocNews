#!/bin/bash

for LANG in en en_GB en_US
do
lupdate -locations relative -no-obsolete engine -ts engine/l10n/ocnewsengine_$LANG.ts
lupdate -locations relative -no-obsolete reader/common reader/harmattan -ts reader/harmattan/l10n/ocnewsreader_$LANG.ts
lupdate -locations relative -no-obsolete reader/common reader/sailfishos -ts reader/sailfishos/l10n/ocnewsreader_$LANG.ts
done

lupdate -locations relative account/harmattan -ts account/harmattan/l10n/ocnewsaccount.ts

