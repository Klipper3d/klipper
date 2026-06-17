#!/bin/bash
# Uninstall script for systemd type installations

# Stop Klipper Service
echo "#### Stopping Klipper Service.."
sudo systemctl stop klipper.service

# Remove Klipper from Startup
echo
echo "#### Removing Klipper from Startup.."
sudo systemctl disable klipper.service

# Remove Klipper from Services
echo
echo "#### Removing Klipper Service.."
sudo rm -f /etc/systemd/system/klipper.service

# Notify user of method to remove Klipper source code
echo
echo "The Klipper system files have been removed."
echo
echo "The following command is typically used to remove local files:"
echo "  rm -rf ~/klippy-env ~/klipper"
