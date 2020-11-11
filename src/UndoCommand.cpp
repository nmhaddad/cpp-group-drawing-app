/**
 *  @file   UndoCommand.cpp
 *  @brief  Implementation of UndoCommand.hpp.
 *  @author John
 *  @date   2020-02-09
 ***********************************************/
#ifndef UNDOCOMMAND_CPP
#define UNDOCOMMAND_CPP

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
#include <string>
#include <cstring>
// Project header files
#include "App.hpp"
#include "UndoCommand.hpp"


/*! \brief Un-execute the previous command by calling <code>App::UndoExecutedCommand()</code>.
 *
 * @return true
 *
 */
bool UndoCommand::silent_execute() {
	app->UndoExecutedCommand();
	return true;
};


/*! \brief UndoCommand does not allow undone commands to be re-undone.  Call RedoCommand instead.
 *
 * @return false.
 *
 */
bool UndoCommand::silent_undo() {
	return false;
};

/*! \brief  This function is a to string method to be passed to network
 *
 * @return A C string of this Command
 */
const char* UndoCommand::parseOut() {
    std::string builder = "6 " + std::string(Command::parseOut());
    char * cstr = new char [builder.length()+1];
    std::strcpy (cstr, builder.c_str());
    return cstr;
};

/** \brief Adjust this command's fields to match the given input.
    e.g. parseIn = "0Generic Command Description\n000\0"
    e.g. parseIn = "0Generic Command Description\n000,50,50,255,255,255;25,25,255,255,255...."
 * @param [in] commandDescription The command as a parsed string.
 **/
UndoCommand* UndoCommand::parseIn(std::string input) {
    // remaining input must be of generic command
    Command::parseIn(input);
    return this;
};

#endif