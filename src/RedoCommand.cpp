/**
 *  @file   RedoCommand.cpp
 *  @brief  Implementation of RedoCommand.cpp
 *  @author John
 *  @date   2020-02-09
 ***********************************************/
#ifndef REDOCOMMAND_CPP
#define REDOCOMMAND_CPP

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
#include <string>
#include <cstring>
// Project header files
#include "App.hpp"
#include "RedoCommand.hpp"

/* \brief Re-execute a previously undone command by calling <code>ExecuteCommand()</code>.
 *
 * @return true.
 *
 */
bool RedoCommand::RedoCommand::silent_execute() {
	app->ExecuteCommand();
	return true;
};


/* \brief RedoCommand does not allow redone commands to be un-redone.  Call UndoCommand instead.
 *
 * @return false.
 *
 */
bool RedoCommand::silent_undo() {
	return false;
};

/*! \brief  This function is a to string method to be passed to network
 *
 * @return A C string of this Command
 */
const char* RedoCommand::parseOut() {
    std::string builder = "5 " + std::string(Command::parseOut());
    char * cstr = new char [builder.length()+1];
    std::strcpy (cstr, builder.c_str());
    return cstr;
};

/** \brief Adjust this command's fields to match the given input.
    e.g. parseIn = "0Generic Command Description\n000\0"
    e.g. parseIn = "0Generic Command Description\n000,50,50,255,255,255;25,25,255,255,255...."
 * @param [in] commandDescription The command as a parsed string.
 **/
RedoCommand* RedoCommand::parseIn(std::string input) {
    // remaining input must be of generic command
    Command::parseIn(input);
    return this;
};

#endif