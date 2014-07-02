cd engine\l10n

FOR %%L IN (en, en_GB, en_US, da, de, es, fr, it, ru, zh_CN) DO (
lrelease ocnewsengine_%%L.ts
)

cd ..\..\account\harmattan\l10n

FOR %%L IN (en, en_GB, en_US, da, de, es, fr, it, ru, zh_CN) DO (
lrelease -markuntranslated '' -idbased ocnewsaccount_%%L.ts
)

cd ..\..\..\reader\harmattan\l10n

FOR %%L IN (en, en_GB, en_US, da, de, es, fr, it, ru, zh_CN) DO (
lrelease ocnewsreader_%%L.ts
)

cd ..\..\sailfishos\l10n

FOR %%L IN (en, en_GB, en_US, da, de, es, fr, it, ru, zh_CN) DO (
lrelease ocnewsreader_%%L.ts
)

cd ..\..\..\