#!/bin/bash
# Uninstall script for raspbian/debian type installations

# Stop Klipper Service
echo "#### Stopping Klipper Service.."
sudo service klipper stop

# Remove Klipper from Startup
echo
echo "#### Removing Klipper from Startup.."
sudo update-rc.d -f klipper remove

# Remove Klipper from Services
echo
echo "#### Removing Klipper Service.."
sudo rm -f /etc/init.d/klipper /etc/default/klipper

# Notify user of method to remove Klipper source code
echo
echo "The Klipper system files have been removed."
echo
echo "The following command is typically used to remove local files:"
echo "  rm -rf ~/klippy-env ~/klipper"
