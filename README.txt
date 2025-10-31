Klipper Firmware — Deployment Guide (Manta M4P v2.0 / Ender 3 V3)

Overview
- Board: BTT Manta M4P v2.0
- MCU: `STM32G0B1`
- Bootloader offset: `8 KiB` (`0x08002000`)
- Clock reference: `8 MHz crystal`
- Communication: `USB on PA11/PA12`

Pinout Summary (alias-based)
- X: `MOTOR0_STEP`, `MOTOR0_DIR`, `MOTOR0_EN`, endstop `X_STOP`
- Y: `MOTOR1_STEP`, `MOTOR1_DIR`, `MOTOR1_EN`, endstop `Y_STOP`
- Z: `MOTOR2_STEP`, `MOTOR2_DIR`, `MOTOR2_EN`, endstop `Z_STOP`
- E: `MOTOR3_STEP`, `MOTOR3_DIR`, `MOTOR3_EN`
- Thermistors: `HOTEND_THERMISTOR`, `BED_THERMISTOR`
- Heaters: `HOTEND_HEATER`, `BED_HEATER`
- Fans: `PART_COOLING`, `HOTEND_FAN`
- Probe: `PROBE_SIGNAL`, `PROBE_SERVO`

Configuration Reference
- Firmware settings are captured in `klipper/.config`:
  - Architecture: STM32
  - Processor: STM32G0B1
  - Clock Reference: 8MHz crystal
  - Communication: USB (PA11/PA12)
  - Bootloader offset: 0x08002000
- Printer config: `klipper/config/manta_m4p_v2_enderv3.cfg`
  - Update `[mcu] serial:` with your device path from `/dev/serial/by-id/`.
  - Calibrate `rotation_distance`, `run_current`, and sensor types for your hardware.

Build (WSL/Ubuntu)
1) Install dependencies:
   - `sudo apt-get update && sudo apt-get install -y make gcc-arm-none-eabi python3 libncurses-dev`
2) Build:
   - `cd /mnt/c/Users/poopo/Documents/trae_projects/manta/klipper`
   - Ensure `.config` exists (provided).
   - `make olddefconfig && make`
3) Output:
   - Firmware: `out/klipper.bin`

Windows Flashing (STM32CubeProgrammer)
1) Enter DFU:
   - Disconnect USB, hold `BOOT`, tap `RESET`, release `BOOT`, reconnect USB.
2) Program:
   - Start address: `0x08002000`.
   - File: `out/klipper.bin`.
   - Click Download; then press `RESET`.
   - Optional CLI: use `klipper/scripts/flash_stm32_usb_dfu.ps1` (requires STM32CubeProgrammer CLI). It connects via `port=USB`, writes at `0x08002000`, verifies, then issues reboot.
3) Alternative:
   - If offset fails, rebuild Klipper with "No bootloader" and flash at `0x08000000`.

Host Integration (Pi/PC)
- Verify serial: `ls /dev/serial/by-id/` and copy the full path into `[mcu]`.
- Restart Klipper service and connect.

Testing Protocol
- Axis: Test X/Y/Z jogs and homing; verify directions and endstops.
- Temperature: Verify hotend/bed readings; PID tune as needed.
- Probe: Deploy/retract and Z-trigger; perform `G28`/`BED_MESH_CALIBRATE`.
- Safety: Test `EMERGENCY_STOP` and max-temp protections.
- Communication: Send G-code, check logs for errors.

Troubleshooting
- USB not detected: try different cable/port, ensure power, re-enter DFU.
- Driver issues: install STM32CubeProgrammer or use Zadig WinUSB for DFU.
- Wrong offset: confirm `.config` bootloader and start address alignment.
- Serial mismatch: always use `/dev/serial/by-id/...` not `/dev/tty*`.
 - Verification: `out/klipper.bin.sha256` contains the SHA256 of the built firmware (generated via `certutil`). Use it to validate the binary before flashing and for post-flash integrity checks.

Version Control
- Do not overwrite critical files; track changes in `CHANGELOG.txt`.
- Record builds in `Deployment_Log.txt`; log test outcomes in `Test_Results.txt`.

Reproducibility Notes
- With `.config` + `manta_m4p_v2_enderv3.cfg` + this README, another technician can rebuild and deploy identical firmware.