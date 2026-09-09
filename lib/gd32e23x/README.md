# GD32E23x device headers

`include/` contains the vendor/CMSIS-compatible GD32E23x device headers used by
the Klipper platform code.  Runtime implementations belong in `src/gd32`.

The headers were copied without content changes from
https://github.com/CrealityOfficial/Ender-3_V3_KE_Klipper at commit
`a63fb1a71672b91b505e0bda68fae2ede49f1168`, path `lib/gd32e23x/`.  They identify
themselves as the GigaDevice GD32E23x 2020-12-12 V1.1.0 header revision and
retain GigaDevice's BSD-3-Clause notice.  Only their directory placement was
changed for this port.
