/**
*  @file   CloseCommand.cpp
*  @brief  Implementation of CloseCommand.hpp
*  @author John
*  @date   2020-02-09
***********************************************/

#ifndef CLOSECOMMAND_CPP
#define CLOSECOMMAND_CPP

#include "App.hpp"
#include "Command.hpp"
#include "CloseCommand.hpp"

/*! \brief  Close the app's window, and by extension, the application.
 *
 * @return True, if the window is successuflly closed.
 *
 */
bool CloseCommand::silent_execute() {
	app->terminate();
	return true;
};

/*! \brief  This function cannot be undone.
 *
 * @return True, if successfully undone.
 */
bool CloseCommand::silent_undo() {
	return false;
};


/*! \brief  This function is a to string method to be passed to network
 *
 * @return A C string of this Command
 */
const char* CloseCommand::parseOut() {
    return "";
};

/** \brief Adjust this command's fields to match the given input.
    e.g. parseIn = "0Generic Command Description\n000\0"
    e.g. parseIn = "0Generic Command Description\n000,50,50,255,255,255;25,25,255,255,255...."
 * @param [in] commandDescription The command as a parsed string.
 **/
CloseCommand* CloseCommand::parseIn(std::string input) {
    // remaining input must be of generic command
    Command::parseIn(input);
    return this;
}


#endif