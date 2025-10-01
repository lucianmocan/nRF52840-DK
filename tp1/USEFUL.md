## Useful commands...

`gmake TARGET=nrf52840 <c_file_name>.upload`

`nrfjprog --recover` if read / write error.

`ls /dev/tty.* /dev/cu.*` to list all available connected devices (on macOS).

`screen <usb_device_path> 115200` to see serial output (on macOS; use `ctrl + a + d` to exit (the process still runs)).

`lsof | grep <usb_device_path>` to find any active processes that use the device.


