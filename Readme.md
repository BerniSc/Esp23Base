# Readme
----------

Base-Project for ESP32 Projects

## How to build and run?
----------

### WSL-Specific
To programm the ESP using WSL we have to manually forward our USB Trafic into WSL as it does not support this out of the box yet.
For this the tool usbipd-win has to be installed on windows-side.
The _.msi_ can be found [here](https://github.com/dorssel/usbipd-win/releases/tag/v4.4.1).
[General stuff here](https://learn.microsoft.com/en-us/windows/wsl/connect-usb#install-the-usbip-tools-and-hardware-database-in-linux)

Then from PowerShell we can run
```PowerShell
usbipd list                         # See the corresponding port
usbipd bind --busid 3-1             # Bind the corresponding port
usbipd attach --wsl --busid 3-1     # Bind the corresponding port
# Optional Detach if we dont want to pull the plug manually so to speak
usbipd detach --busid 3-1
```

On WSL run
```bash
sudo hwclock -s
```
to avoid SSL Issues

The device should now show up on `lsusb` as something like this:<br>
_Bus 001 Device 002: ID 10c4:ea60 Silicon Labs CP210x UART Bridge_

## NVim
Generate compilecommands for LSP by calling
```
idf.py reconfigure
```
