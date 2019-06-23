#!/bin/bash

#Require sudo
if [ $EUID != 0 ]; then
    sudo "$0" "$@"
    exit $?
fi

echo "setting to /usr/local/bin/fan-daemon/..."
rm -rf /usr/bin/fan-daemon/ 2>/dev/null
mkdir /usr/local/bin/fan-daemon
cp ./bin/Release/fan-daemon /usr/local/bin/fan-daemon/
echo "done"

echo "adding service to /lib/systemd/system/..."
cp fan-daemon.service /lib/systemd/system/
chmod 644 /lib/systemd/system/fan-daemon.service
echo "done"

echo "starting and enabling service..."
systemctl daemon-reload
systemctl start fan-daemon
systemctl enable fan-daemon
echo "done"

echo "fan-daemon installed sucessfully!"
