<#
Purpose: Flash Klipper firmware to STM32 (Manta M4P v2.0) via USB DFU using STM32CubeProgrammer CLI.

Prereqs:
- Install STM32CubeProgrammer (includes STM32_Programmer_CLI.exe)
- Board in DFU mode (BOOT0+RST per WINDOWS_FLASHING.md)
- Firmware at ..\out\klipper.bin built with start address 0x08002000

Usage (PowerShell):
  .\flash_stm32_usb_dfu.ps1

Note:
- Adjust `$CLI`, `$PORT`, and `$ADDRESS` if needed.
#>

Param(
  [string]$BinPath = "$PSScriptRoot\..\out\klipper.bin",
  [string]$CLI = "C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe",
  [string]$Port = "USB",
  [string]$Address = "0x08002000"
)

Write-Host "Flashing firmware via STM32CubeProgrammer CLI" -ForegroundColor Cyan
Write-Host "Bin: $BinPath" -ForegroundColor Cyan
Write-Host "CLI: $CLI" -ForegroundColor Cyan
Write-Host "Port: $Port, Address: $Address" -ForegroundColor Cyan

if (!(Test-Path $BinPath)) {
  Write-Error "Firmware not found: $BinPath"
  exit 1
}
if (!(Test-Path $CLI)) {
  Write-Error "STM32_Programmer_CLI.exe not found. Please install STM32CubeProgrammer and update path."
  exit 1
}

# Connect to device in DFU mode and write binary at the specified address with verify
& $CLI -c port=$Port -w $BinPath $Address -v
if ($LASTEXITCODE -ne 0) {
  Write-Error "Flashing failed (exit code $LASTEXITCODE)."
  exit $LASTEXITCODE
}

Write-Host "Flash completed and verified successfully." -ForegroundColor Green

# Optional: reboot device if supported
& $CLI -c port=$Port -rst
Write-Host "Reboot command issued." -ForegroundColor Green