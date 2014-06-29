#!/bin/sh

if [ "$(pidof ocNewsReader)" ]; then
 dbus-send --dest=de.buschmann23.ocNewsReader --print-reply / de.buschmann23.ocNewsReader.showContent array:string:$1; else
 /usr/bin/invoker --splash=/opt/ocNewsReader/images/splash.jpg --splash-landscape=/opt/ocNewsReader/images/splash-l.jpg --type=d -s /opt/ocNewsReader/bin/ocNewsReader $1; fi
