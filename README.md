# OpenVPN ubus
OpenWRT package which enables user to control OpenVPN management server through ubus. Communication happens through sockets. The ubus process lets the user get the status of connected clients and also disconnect them.

## Usage
After starting a server with management enabled the ubus process will have an object `openvpn.SERVER_NAME` and two methods.
- "status":{}
-	"kill":{"common_name":"String"}

## Notes
- OpenWRT needs to be compiled with OpenVPN management server (disabled by default).
- The ubus process starts only through OpenVPN `init.d` script which reads an OpenVPN UCI configuration.
- The OpenVPN UCI configuration needs to have `option management 'IP_ADDR PORT'` for management server to start and expose it to ubus process.

## OpenVPN package notes
- This package replaces the original OpenVPN `.init` script at `/etc/init.d/openvpn` with a slightly modified version to accomodate this package's functionality.
- There is an original copy of an OpenVPN `.init` script at `src/files/openvpn.init` and it's patch at `patches/` directory.
