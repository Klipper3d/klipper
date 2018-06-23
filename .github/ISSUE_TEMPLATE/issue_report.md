---
name: Issue report
about: Create a report to help us improve

---

Issue reporting
===============

In order to report a problem or request a change in behavior, it is
necessary to collect the Klipper log file. The first step is to
**issue an M112 command** in the OctoPrint terminal window immediately
after the undesirable event occurs. This causes Klipper to go into a
"shutdown state" and it will cause additional debugging information to
be written to the log file.

Issue requests are submitted through Github.  **All issues must
include the full /tmp/klippy.log log file from the session that
produced the error.** An "scp" and/or "sftp" utility is needed to
acquire this log file. The "scp" utility comes standard with Linux and
MacOS desktops. There are freely available scp utilities for other
desktops (eg, WinSCP).

Use the scp utility to copy the `/tmp/klippy.log` file from the host
machine to your desktop. It is a good idea to compress the klippy.log
file before posting it (eg, using zip or gzip). Open a new issue at
https://github.com/KevinOConnor/klipper/issues , provide a description
of the problem, and **attach the `klippy.log` file to the issue**:
![attach-issue](img/attach-issue.png)

**Describe the bug**
A clear and concise description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '....'
3. Scroll down to '....'
4. See error

**Expected behavior**
A clear and concise description of what you expected to happen.

**Screenshots**
If applicable, add screenshots to help explain your problem.

**Desktop (please complete the following information):**
 - OS: [e.g. iOS]
 - Browser [e.g. chrome, safari]
 - Version [e.g. 22]

**Additional context**
Add any other context about the problem here.

