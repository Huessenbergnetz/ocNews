#!/bin/bash

#for LANG in de en_GB fr zh_CN
for LANG in en_GB
do
lupdate -no-obsolete engine -ts translations/common/ocnewsengine_$LANG.ts
lupdate account/harmattan -ts translations/harmattan/ocnewsaccount_$LANG.ts
lupdate -no-obsolete reader/common reader/harmattan -ts translations/harmattan/ocnewsreader_$LANG.ts
lupdate -no-obsolete reader/common reader/sailfishos -ts translations/sailfishos/ocnewsreader_$LANG.ts
done

