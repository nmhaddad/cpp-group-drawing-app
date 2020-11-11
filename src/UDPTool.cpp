/**
 *  @file   UDPTool.cpp
 *  @brief  Handles udp server and client code.
 *  @author John and Duncan; adapted in part from lab8
 *  @date   2020-03-04
 ***********************************************/
#ifndef NETWORK_TOOLS_CPP
#define NETWORK_TOOLS_CPP

#include "UDPTool.hpp"
#include <chrono>
#include <thread>
#include <cstring>


UDPTool::UDPTool(sf::IpAddress targetIp, unsigned short targetPort, unsigned short ownPort, bool isServer) {

    // If we have a client to register
    if (targetPort != 0) {
        registerNewClient(targetIp, targetPort);
    }
    this->ownPort = ownPort;
    this->isServer = isServer;

    // setup buffer
    memset(buffer, 0, 2000);
    recievedMessageSize = 0;

    // setup socket
    socket.bind(ownPort);
    if (isServer) {
        std::cout << "============Initiating UDP Server==========" << std::endl;
    }
    else {
        std::cout << "============Initiating UDP Client==========" << std::endl;
    }

    // test port
    if (socket.send(" ", 2, sf::IpAddress("127.0.0.1"), ownPort) != sf::Socket::Done) {
        std::cout << "Error attaching to port.\nMake sure port is not being used by another program." << std::endl;
        std::cout << "Please restart program." << std::endl;
        while (true);
    }
    else {
        std::cout << "Port attached successfully." << std::endl;
    }
    socket.setBlocking(false);
};

 /*! \brief     Ask the user for port and ip information, then return a client object.
 *
 */
UDPTool* UDPTool::getClient() {

    // ask for client details
    std::string clientPort = "";
    std::string got = "n";
    do {
        std::cout << "Set client port number: " << std::endl;
        std::getline(std::cin, clientPort);

        std::cout << "Verify port entered correctly (y/n): " << clientPort << std::endl;
        do {
            std::getline(std::cin, got);
        } while (sizeof(got) == 0);
    } while (std::tolower(got[0]) == 'n');


    // ask for server details
    std::string serverIp = "";
    std::string serverPort = "";
    do {
        std::cout << "Set target server ip: " << std::endl;
        // since getline was used before another cin, we need to ignore a leading whitespace character
        std::getline(std::cin, serverIp);

        std::cout << "Set target server port: " << std::endl;
        std::getline(std::cin, serverPort);

        std::cout << "Verify correct (y/n): " << serverIp << ":" << serverPort << std::endl;
        do {
            std::getline(std::cin, got);
        } while(sizeof(got) == 0);
    } while (std::tolower(got[0]) == 'n');
    return UDPTool::getClient(serverIp, serverPort, clientPort);
};


/*! \brief      Generate and return a client object.
*
*/
UDPTool* UDPTool::getClient(std::string serverIp, std::string serverPort, std::string ownPort) {
    return new UDPTool(sf::IpAddress(serverIp.c_str()), std::stoi(serverPort), std::stoi(ownPort), false);
};

/*! \brief      Ask the user for port information, then return a server object.
*
*/
UDPTool* UDPTool::getServer() {
    std::string port = "";
    std::string got = "n";
    do {
        std::cout << "Set server port number: " << std::endl;
        std::getline(std::cin, port);

        std::cout << "Verify port entered correctly (y/n): " << port << std::endl;
        do {
            std::getline(std::cin, got);
        } while (sizeof(got) == 0);
    } while (std::tolower(got[0]) == 'n');
    return getServer(port);
};

/*! \brief      Generate and return a server object.
*
*/
UDPTool* UDPTool::getServer(std::string ownPort) {
    // server does not care about what port users see.
    // client ports are stored in computerID
    return new UDPTool(sf::IpAddress(), 0, std::stoi(ownPort), true);
};


/*! \brief      Given some message, send it.
*   
* If this is a client, send it to the client's targetted server.
*
* If this is a server, send it to all clients.
*/
void UDPTool::send(std::string message) {
    sendExcluding(message);
};

/*! \brief      Given some message, send it, except to the excludePort.
*
* If this is a client, throw "Illegal State: client cannot call this function".
*
* If this is a server, send it to all clients except for exclude_port.
*/
void UDPTool::sendExcluding(std::string message, unsigned short excludePort) {

#ifdef _DEBUG
    std::cout << "Broadcasting: " << message << std::endl;
    if (excludePort != 0) {
        std::cout << "    Excluded: " << excludePort << std::endl;
    }
#endif

    std::map<unsigned short, sf::IpAddress>::iterator innerIterator;
    // for each computer ID ...
    for (innerIterator = computerID.begin(); innerIterator != computerID.end(); ++innerIterator) {
        // that is not excluded ...
        if (innerIterator->first != excludePort) {
            // send the message

#ifdef _DEBUG
            std::cout << "     Sent to: " << innerIterator->second << ":" << innerIterator->first << std::endl;
#endif
            socket.send(message.c_str(), message.size(), innerIterator->second, innerIterator->first);
        }
    }
};

/*! \brief      Given a client, register it.
*
* Check if a client is already registered; if not, register them then return true.
*/
bool UDPTool::registerNewClient(sf::IpAddress connectingClientIP, unsigned short connectingClientPort) {

    if (computerID.find(connectingClientPort) == computerID.end()) {
        std::cout << "New contact registered: "
            << connectingClientIP
            << ":"
            << connectingClientPort
            << std::endl;
        computerID[connectingClientPort] = connectingClientIP;
        return true;
    }
    return false;
};

/*! \brief      Send all commands recieved so far to some client.
*  Sleep code learned here:
*  https://stackoverflow.com/questions/158585/how-do-you-add-a-timed-delay-to-a-c-program
*/
void UDPTool::sendAllCommands(unsigned short connectingClientPort) {
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds

    std::cout << "sendAllCommands: Bringing user " << connectingClientPort << " up to date.\n";
    std::vector<std::string>::iterator commandIterator;
    // send each command
    for (commandIterator = commandList.begin(); commandIterator != commandList.end(); ++commandIterator) {
        std::string message = *commandIterator;
        socket.send(message.c_str(), message.size(), computerID[connectingClientPort], connectingClientPort);
        sleep_for(milliseconds(5));
    }
};


/*! \brief      If there's a message in the buffer, return it; else, return nullptr.
*
* If this is the server, also register the incoming messanger's ip and port, and send them all the messages so far.
*
* If viaPort is used, store the messanger's port there.
*
*/
std::string UDPTool::recieve() {
    unsigned int viaPort;
    return recieve(viaPort);
}
std::string UDPTool::recieve(unsigned int& viaPort) {
    std::string got = "";
    sf::IpAddress connectingIP;
    unsigned short connectingPort;
    socket.receive(buffer, 2000, recievedMessageSize, connectingIP, connectingPort);

    got = std::string(buffer);
    memset(buffer, 0, 2000);

    if (got.length() > 0) {
        viaPort = connectingPort;
        if (isServer) {
            std::cout << "Contacted by " << connectingIP << ":" << connectingPort << std::endl;
        }

        if (connectingPort == this->ownPort) {
            // is this the server's first self directed packet?
            if (!selfContact) {
                selfContact = true;
#ifdef _DEBUG
                std::cout << "Successfully contacted self via local ip." << std::endl;
#endif
                return "";
            }
            if (isServer) {
                std::cout << "A client attempted to join with server's port."
                    << "\nThis is not currently supported." << std::endl;
            }
            else {
                std::cout << "Recieved message from own port."
                    << "\nCheck port setup." << std::endl;
            }
            return "";
        }
        if (isServer) {
            if (registerNewClient(connectingIP, connectingPort)) {
                sendAllCommands(connectingPort);
            }
        }

#ifdef _DEBUG
        std::cout << "Received: " << got << std::endl;
#endif
    }

    return got;
};

/*! \brief      Ask the user if the application is a server or client.
*
*/
bool UDPTool::askUserIfServer() {
    // Stores a role of either a server or client user.
    std::string role = "";

    do {
        std::cout << "Enter (s) for Server, Enter (c) for client: " << std::endl;

        do {
            std::getline(std::cin, role);
        } while (role.length() == 0);
    } while (std::tolower(role[0]) != 's' && std::tolower(role[0]) != 'c');

    if (std::tolower(role[0]) == 's') {
        return true;
    }
    else {
        return false;
    }
};

/*! \brief      Perform all server operations.
*
*/
void UDPTool::serverGo() {
    unsigned int clientPort;
    unsigned int commandNumber = 0;
    while (true) {
        // check for incoming messages
        std::string incoming = recieve(clientPort);

        // filter server level commands
        if (incoming == "Join attempt.") {
            //do a server thing, potentially
            incoming = "";
        }

        if (incoming.size() > 0) {
            // new client has already been registered and brought up to date in recieve.
            // attach a command number to the front of the message to keep it in order.
            #ifdef USE_CASTING // use type casting
                incoming = std::to_string((char*)&commandNumber) + incoming.size();
            #endif
            #ifndef USE_CASTING // don't use type casting
                // convert the command to string
                // incoming = std::to_string(commandNumber) + " " + incoming;
                // incoming = incoming;
            #endif

            // log the new command for posterity.
            commandList.push_back(incoming);
            commandNumber++;

            // forward the recieved command to all other clients
            sendExcluding(incoming, clientPort);
        }
    }
};

void UDPTool::clientMessageTest() {
    // Main UDP loop
    while (true) {
        std::string sendText = "";
        // get user input; use getline to avoid endline
        std::cout << ">";
        std::getline(std::cin, sendText);

        // send to the server
        send(sendText.c_str());

        // check for new messages
        std::string recieved = recieve();
    }// Main UDP loop
}


#endif