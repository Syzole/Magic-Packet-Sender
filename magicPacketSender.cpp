#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <cstdio>
#include "json.hpp"
#include <fstream>
#include <string>

using namespace std;

// for convenience
using json = nlohmann::json;

void initializeWinsock()
{
    // make winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        cerr << "WSAStartup failed: " << result << endl;
        exit(1);
    }
}

void convertMacAddress(const char *macStr, unsigned char *mac)
{
    // temporary array to store the 6 bytes of the MAC address
    // networks use big-endian byte order, so we need to convert the hex bytes to unsigned char(good note to remember)
    unsigned int macBytes[6];
    if (sscanf(macStr, "%x:%x:%x:%x:%x:%x",
               &macBytes[0], &macBytes[1], &macBytes[2],
               &macBytes[3], &macBytes[4], &macBytes[5]) != 6)
    {
        cerr << "Invalid MAC address format." << endl;
        exit(1);
    }
    for (int i = 0; i < 6; ++i)
    {
        // convert to unsigned char to store in mac array
        mac[i] = static_cast<unsigned char>(macBytes[i]);
    }

    // cout << "MAC address: " << macStr << " -> ";
    // for (int i = 0; i < 6; ++i)
    // {
    //     cout << hex << static_cast<int>(mac[i]);
    //     if (i < 5)
    //     {
    //         cout << ":";
    //     }
    // }

    // cout << endl;
}

void createMagicPacket(unsigned char *mac, unsigned char *packet)
{
    // 6 bytes of 0xFF
    for (int i = 0; i < 6; ++i)
    {
        packet[i] = 0xFF;
    }
    // 16 repetitions of the MAC address
    for (int i = 1; i <= 16; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            packet[i * 6 + j] = mac[j];
        }
    }
}

void sendMagicPacket(const char *ipAddress, int port, unsigned char *packet, int packetSize)
{   

    //this part was the most difficult to get working -_-

    // Create a socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET)
    {
        cerr << "Socket creation failed: " << WSAGetLastError() << endl;
        WSACleanup();
        exit(1);
    }

    // Set the socket as broadcast-capable
    sockaddr_in targetAddr;
    targetAddr.sin_family = AF_INET;

    // Set the broadcast address
    targetAddr.sin_port = htons(port);

    // Set the IP address
    inet_pton(AF_INET, ipAddress, &targetAddr.sin_addr.s_addr);

    // Send the Magic Packet
    int result = sendto(sock, reinterpret_cast<const char *>(packet), packetSize, 0, (sockaddr *)&targetAddr, sizeof(targetAddr));
    if (result == SOCKET_ERROR)
    {
        cerr << "sendto failed: " << WSAGetLastError() << endl;
    }   

    closesocket(sock);
}

void handleMagicPacket(const char *ipAddress, const char *macStr, int port)
{
    // Initialize Winsock
    initializeWinsock();

    // Convert MAC address string to byte array
    unsigned char mac[6];
    convertMacAddress(macStr, mac);

    // Create the Magic Packet
    unsigned char packet[102];
    createMagicPacket(mac, packet);

    // Send the Magic Packet
    sendMagicPacket(ipAddress, port, packet, sizeof(packet));

    // Cleanup Winsock
    WSACleanup();

}

int main()
{
    ifstream i("config.json");
    json data = json::parse(i);

    if(data.empty())
    {
        cout << "Error: No data found in the config file!" << endl;
        return 1;
    }

    // Target MAC address in string format
    string macAdd = data["macAddress"];

    // Convert MAC address string to char array
    const char *macStr = macAdd.c_str();

    // IP address of the target machine (or the broadcast address)
    const char *ipAddress = data["ipAddress"].get<std::string>().c_str();

    // Port number to send the Magic Packet to (default is 9 or 7)
    int port = data["port"];

    // Handle the Magic Packet sending(helper function)
    handleMagicPacket(ipAddress, macStr, port);

    cout << "Process Complete!" << endl;

    return 0;
}
