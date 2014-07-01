#!/bin/bash

for LANG in en en_GB en_US
do
lupdate -target-language $LANG -locations relative -no-obsolete engine -ts engine/l10n/ocnewsengine_$LANG.ts
lupdate -target-language $LANG -locations relative -no-obsolete reader/common reader/harmattan -ts reader/harmattan/l10n/ocnewsreader_$LANG.ts
lupdate -target-language $LANG -locations relative -no-obsolete reader/common reader/sailfishos -ts reader/sailfishos/l10n/ocnewsreader_$LANG.ts
lupdate -target-language $LANG -locations relative account/harmattan -ts account/harmattan/l10n/ocnewsaccount_$LANG.ts
done

lupdate -source-language en -locations relative account/harmattan -ts account/harmattan/l10n/ocnewsaccount.ts
lupdate -source-language en -locations relative -no-obsolete engine -ts engine/l10n/ocnewsengine.ts
lupdate -source-language en -locations relative -no-obsolete reader/common reader/harmattan -ts reader/harmattan/l10n/ocnewsreader.ts
lupdate -source-language en -locations relative -no-obsolete reader/common reader/sailfishos -ts reader/sailfishos/l10n/ocnewsreader.ts
