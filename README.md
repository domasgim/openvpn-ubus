# OpenVPN ubus
OpenWRT package which enables user to control OpenVPN management server through ubus. Communication happens through sockets. The ubus process lets the user get the status of connected clients and also disconnect them.

## Usage
After starting a server with management enabled the ubus process will have an object `openvpn.SERVER_NAME` and two methods.
- "status":{}
-	"kill":{"common_name":"String"}

## Notes
- OpenWRT needs to be compiled with OpenVPN management server (disabled by default).
- Package replaces the original OpenVPN init.d script at `/etc/init.d/openvpn`.
- You can manually modify the init.d script to not overwrite the original by appending scpecified lines found in `files/sample_openvpn.init` to the `start_instance()` method.
- The ubus process starts only through OpenVPN init.d script which reads an OpenVPN UCI configuration.
- The OpenVPN UCI configuration needs to have `option management 'IP_ADDR PORT'` for management server to start and expose it to ubus process. Sample server configuration provided at `files/sample_openvpn_uci`
