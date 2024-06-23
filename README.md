## Wake-On-LAN (WOL) Magic Packet Sender

### Description:
This script sends a Wake-On-LAN (WOL) magic packet to a target device. The target device must have WOL enabled in the BIOS and the network card must support WOL. The target device does not need to on the same network as the sender. The target device must be connected to a network that is routable from the sender. You must port forward the WOL port (default 9) to the target device if the target device is on a different network. I would recommend combining this moonlight internet hosting tool to automate port forwarding if you want to use this script outside of the network.

### Instructions:

1. Download the script.
2. Open or create a config.json file in the same directory as the script.
3. Add the following to the config.json file:
```json
{
    "ipAddress": "Global.Ip.Address",
    "macAddress": "AB:CD:EF:GH:IJ:KL",
    "port": 9
}
```

where "mac" is the MAC address of the target device, "ip" is the IP address of the target device, and "port" is the port number to send the magic packet to.
If you want to use the script outside of the network of the computer dwyou must provide the global IP address of the target device not the local IP address.


## To learn more:
https://www.pdq.com/blog/wake-on-lan-wol-magic-packet-powershell/


### Note:
You may see a make test function in the script. This file was used to test diffrent packages and functions to see which one would work best. 

