 #!/bin/bash
 
STARTDIR=$PWD

cd translations/common
lrelease *.ts
cd ../harmattan
lrelease -markuntranslated '' -idbased ocnewsaccount*.ts
lrelease ocnewsreader*.ts
cd ../sailfishos
lrelease *.ts