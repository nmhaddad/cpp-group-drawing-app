/**
*  @file   ChatCommand.cpp
*  @brief  Implementation of ChatCommand.hpp
*  @author Duncan
*  @date   2020-04-09
***********************************************/
#ifndef CHATCOMMAND_CPP
#define CHATCOMMAND_CPP

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
// #include ...
// Project header files
#include "Command.hpp"
#include "ChatCommand.hpp"
// #include "UPDTool.hpp"
#include "UDPTool.hpp"
#include <string>
#include <cstring>
#include <iostream>

/*! \brief  Print message to console.
 *
 * @return True, if the message is successfully sent.
 */
bool ChatCommand::silent_execute() {

	if (message.length() == 0) {
		std::cout << "Type your message to send to all the other users here: " << std::endl;
		std::cout << "> " ;
		std::string userMessage = "";
		getline(std::cin, userMessage);
		message = userMessage;
	} else {
		std::size_t clientNameStart = message.find("/*/*/");
		std::string sendingClientName = message.substr(clientNameStart + 5);
		std::string parsedMessage = message.substr(0, clientNameStart);
		std::cout << "Chat: " << sendingClientName << "> " << parsedMessage << std::endl;
	}
	return true;
};


/*! \brief  This function cannot be undone.
 *
 * @return True, if successfully undone.
 */
bool ChatCommand::silent_undo() {
	return false;
};


/*! \brief  This function is a to string method to be passed to network
 *
 * @return A C string of this Command
 */
const char* ChatCommand::parseOut() {
    std::string builder = "7 " + message + "/*/*/" + app->connectingClientName;
    char * cstr = new char [builder.length()+1];
    std::strcpy (cstr, builder.c_str());
    return cstr;
};

/** \brief Adjust this command's fields to match the given input.
 * @param [in] commandDescription The command as a parsed string.
 **/
ChatCommand* ChatCommand::parseIn(std::string input) {

	// Turn character array into string
    std::string parseString = input;
        
    // Erase first 2 characters that just indicate the command type
    parseString.erase(0,2);

    message = parseString;

    return this;
}

#endif