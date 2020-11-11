/**
 *  @file   ChatCommand.hpp
 *  @brief  User command to send a message to all other clients.
 *  @author Duncan and team
 *  @date   2020-04-09
 ***********************************************/
#ifndef CHATCOMMAND_HPP
#define CHATCOMMAND_HPP

//SFML libraries
#include <SFML/Graphics.hpp>
//Local libraries
#include "App.hpp"
#include "Command.hpp"
#include "UDPTool.hpp"
#include <string>

/** \brief Set the current message.
*/
class ChatCommand : public Command {
private:
	std::string message;		//!< The new message for this command.
	bool silent_execute() override;
	bool silent_undo() override;
	ChatCommand();
public:

    /*! \brief  Create a chat command.
     *
     * @return a chat command object.
     */
    ChatCommand(App* app) : Command("Chat command.", app) {commandType = CHAT; };

    ChatCommand* parseIn(std::string input) override;
    const char* parseOut() override;

};

#endif