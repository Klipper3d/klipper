# Welcome to the Klipper for four axis project!

[![Klipper](docs/img/klipper-logo-small.png)](https://www.klipper3d.org/)

https://www.klipper3d.org/

This Klipper Version is a 3d-Printer firmware which enables the use of 4 indipendent Axis. Klipper combines the power of a general
purpose computer with one or more micro-controllers. See the
[origninal Klipper features document](https://www.klipper3d.org/Features.html) for more
information on why you should use Klipper.


Klipper is Free Software. See the [license](COPYING) or read the
[documentation](https://www.klipper3d.org/Overview.html). We depend on
the generous support from our
[sponsors](https://www.klipper3d.org/Sponsors.html).


# Installation

The easiest way is to use a KIAUH "klipper_repos.txt" file. Details at: <https://github.com/th33xitus/kiauh/blob/master/klipper_repos.txt.example>

1. SSH into the Pi.
2. Copy "klipper_repos.txt.example" to "klipper_repos.txt".
    - Use the command: `cp kiauh/klipper_repos.txt.example  kiauh/klipper_repos.txt`
4. Edit the `kiauh/klipper_repos.txt` file to append "`Thomas-Toucan3d/klipper,master`" after the last line.
    - Use the command: `echo "Thomas-Toucan3d/klipper,master" >> kiauh/klipper_repos.txt`
5. Start KIAUH.
    - Use the command: `./kiauh/kiauh.sh`
7. Choose option "`6) [Settings]`".
8. Choose option "`1) Set custom Klipper repository`".
9. Choose the option corresonding to "`Thomas-Toucan3d/klipper -> master`"
10. Use KIAUH to uninstall and reinstall Klipper.
11. Have fun!