#!/bin/bash

for LANG in en_GB
do
lupdate -no-obsolete engine -ts engine/l10n/ocnewsengine_$LANG.ts
lupdate account/harmattan -ts account/harmattan/l10n/ocnewsaccount_$LANG.ts
lupdate -no-obsolete reader/common reader/harmattan -ts reader/harmattan/l10n/ocnewsreader_$LANG.ts
lupdate -no-obsolete reader/common reader/sailfishos -ts reader/sailfishos/l10n/ocnewsreader_$LANG.ts
done

