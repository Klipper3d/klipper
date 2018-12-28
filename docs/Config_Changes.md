This document covers recent software changes to the config file that
are not backwards compatible. It is a good idea to review this
document when upgrading the Klipper software.

All dates in this document are approximate.

# Changes

20190107: The i2c_address parameter in the mcp4451 config section
changed. This is a common setting on Smoothieboards. The new value is
half the old value (88 should be changed to 44, and 90 should be
changed to 45).

20181220: Klipper v0.7.0 released
