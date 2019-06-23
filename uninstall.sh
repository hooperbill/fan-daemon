#!/bin/bash

#Require sudo
if [ $EUID != 0 ]; then
    sudo "$0" "$@"
    exit $?
fi

echo "removing service..."
systemctl stop fan-daemon
systemctl disable fan-daemon
echo "done"


echo "removing /usr/local/bin/fan-daemon/..."
rm -r /usr/local/bin/fan-daemon
rm -r /usr/bin/fan-daemon 2>/dev/null
echo "done"

echo "removing service from /lib/systemd/system/..."
rm /lib/systemd/system/fan-daemon.service
echo "done"

echo "reloading services"
systemctl daemon-reload
echo "done"

echo "fan-daemon uninstalled sucessfully!"
echo ""
echo "If you are find a problem, please create an issue at the repo."
