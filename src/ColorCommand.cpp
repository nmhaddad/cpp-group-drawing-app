/**
*  @file   ColorCommand.cpp
*  @brief  Implementation of ColorCommand.hpp
*  @author John
*  @date   2020-02-09
***********************************************/
#ifndef COLORCOMMAND_CPP
#define COLORCOMMAND_CPP

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
// #include ...
// Project header files
#include "App.hpp"
#include "Command.hpp"
#include "ColorCommand.hpp"


/*! \brief  Set the application color to a new color.
 *
 * @return True, if the color is successfully set.
 */
bool ColorCommand::silent_execute() {
	app->color = new sf::Color(color->r, color->g, color->b, color->a);
	return true;
};

/*! \brief  Sets the application color to the previous color.
 *
 * @return True, if the color is successfully set.  True, if the command is successfully undone.
 *
 */
bool ColorCommand::silent_undo() {
	app->color = new sf::Color(lastColor->r, lastColor->g, lastColor->b, lastColor->a);
	return true;
};

/*! \brief  This function is a to string method to be passed to network
 *
 * @return A C string of this Command
 */
const char* ColorCommand::parseOut() {
    return "";
};

/** \brief Adjust this command's fields to match the given input.
    e.g. parseIn = "0Generic Command Description\n000\0"
    e.g. parseIn = "0Generic Command Description\n000,50,50,255,255,255;25,25,255,255,255...."
 * @param [in] commandDescription The command as a parsed string.
 **/
ColorCommand* ColorCommand::parseIn(std::string input) {
    // remaining input must be of generic command
    Command::parseIn(input);
    return this;
}

#endif