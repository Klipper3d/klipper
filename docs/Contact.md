This page provides information on how to contact the Klipper
developers.

Bug reporting
=============

Bug reports are submitted through github issues.  All bug reports must
include the full /tmp/klippy.log log file from the session that
produced the error.  To acquire this log file, ssh into the computer
running the klipper host software, and run:

```
gzip -k /tmp/klippy.log
```

Then scp the resulting `/tmp/klippy.log.gz` file from the host machine
to your desktop. (If your desktop does not have scp installed, there
are a number of free scp programs available - just do a web search for
`windows scp` to find one.) Open a new issue at
https://github.com/KevinOConnor/klipper/issues , provide a description
of the problem, and attach the `klippy.log.gz` file to the issue (use
the blue "selecting them" link beneath the problem description on the
github issue page to attach files).

If the Klipper software does something unexpected but does not produce
an error, then immediately issue an M112 command in the Octoprint
terminal window and then gather the log as described above. The M112
command instructs Klipper to enter a "shutdown" state and it will
cause additional debugging information to be added to the log.

Mailing list
============

There is a mailing list for general discussions on Klipper. In order
to send am email to the list, one must first subscribe:
https://www.freelists.org/list/klipper . Once subscribed, emails may
be sent to `klipper@freelists.org`.

Archives of the mailing list are available at:
https://www.freelists.org/archive/klipper/

IRC
===

One may join the #klipper channel on freenode.net (
irc://chat.freenode.net:6667 ).

To communicate in this IRC channel one will need an IRC
client. Configure it to connect to chat.freenode.net on port 6667 and
join the #klipper channel (`/join #klipper`).

If asking a question on IRC, be sure to ask the question and then stay
connected to the channel to receive responses. Due to timezone
differences, it may take several hours before receiving a response.
