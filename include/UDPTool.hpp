/**
 *  @file   UDPTool.hpp
 *  @brief  Handles udp server and client code.
 *  @author John and Duncan; adapted in part from lab8
 *  @date   2020-03-04
 ***********************************************/
#ifndef UDPTOOL_HPP
#define UDPTOOL_HPP

 // Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Network.hpp>

// Include standard library C++ libraries.
#include <queue>
#include <stack>
#include <iostream>

// Project header files
#include "Command.hpp"

const size_t UDPTOOL_BUFFER_SIZE = 2000;
// #define USE_CASTING;

class UDPTool {
private:
    bool isServer;
    sf::UdpSocket socket;
    unsigned short ownPort;
    std::map<unsigned short, sf::IpAddress> computerID; // <port#,ipaddress>

    char buffer[2000];
    std::size_t recievedMessageSize;

    // Server relevant attributes
    bool selfContact = false;
    bool registerNewClient(sf::IpAddress connectingClientIP, unsigned short connectingClientPort);
    void sendAllCommands(unsigned short connectingClientPort);
    void sendExcluding(std::string message, unsigned short excludePort=0);
    std::vector<std::string> commandList;

    // hide constructor to force use of artificial constructors
    UDPTool(sf::IpAddress targetIp, unsigned short targetPort, unsigned short ownPort, bool isServer);
    // delete copy constructor and move operator; connected socket must remain unique
    UDPTool(const UDPTool&) = delete;
    UDPTool& operator=(const UDPTool&) = delete;
public:
    // artificial constructors to ensure coder gets client or server as needed
    static UDPTool* getClient();
    static UDPTool* getClient(std::string serverIp, std::string serverPort, std::string ownPort);
    static UDPTool* getServer();
    static UDPTool* getServer(std::string ownPort);
    static bool askUserIfServer();

    void send(std::string message);
    std::string recieve();
    std::string recieve(unsigned int& viaPort);
    void serverGo();
    void clientMessageTest();
};

#endif