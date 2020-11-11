/** 
 *  @file   Draw.cpp 
 *  @brief  Implementation of DrawCommand.hpp.
 *  @author Mike and John
 *  @date   2020-02-09 
 ***********************************************/
#ifndef DRAW_CPP
#define DRAW_CPP

// Include our Third-Party SFML header
#include <SFML/Graphics/Color.hpp>
// Include standard library C++ libraries.
#include <iostream>
#include <cstring>
// Project header files
#include "App.hpp"
#include "DrawCommand.hpp"


/*! \brief Draw a line along user mouse inputs.
 *	<p> If the command has not been performed, start a new line of
 *		the application's current color at the current mouse position.
 *		The command will then become <code>active</code>.
 *		Subsequent calls will draw the line along new mouse positions.
 *	<p>	If the command is called after it has been terminated (i.e. when it
 *		has been placed in the undo stack, after the user releases their click)
 *		it will call terminate, which will push the previously created
 *		line to the application.
 *	<p>	This command cannot be undone until this function is called and
 *		subsequently terminated.
 *
 * @return True, if the command is successfully run.
 *
 */
bool DrawCommand::silent_execute() {
	if (redoAble) {

#ifdef _DEBUG
        std::cout << "\nDRAW COMMAND CALLED TERMINATE ON " << get_m_commandDescription() << "\n";
#endif
		terminate();
		return true;
	}

	if (!active) {
		// first execution
		active = true;
		drawable = new std::vector<sf::VertexArray*>();
		// create first pixel point
		drawable->push_back(new sf::VertexArray());
		drawable->back()->append(sf::Vertex(sf::Vector2f((float) app->mouseX, (float) app->mouseY), *app->color));

		// create first line point
		drawable->push_back(new sf::VertexArray(sf::LineStrip, 0));
	}

    // first and continued executions
	if (app->mouseX != app->pmouseX || app->mouseY != app->pmouseY) {
		// as long as our position has changed, add a new line
		drawable->back()->append(sf::Vertex(sf::Vector2f((float) app->mouseX, (float) app->mouseY), *app->color));

	}
	return true;
}


/*! \brief Check if the user has stopped clicking; terminate if so.
 *
 */
void DrawCommand::inform(sf::Event event) {
	if (event.type == sf::Event::MouseButtonReleased) {

#ifdef _DEBUG
		std::cout << "Terminated: " << get_m_commandDescription() << std::endl;
#endif
		terminate();
	}
};


/*! \brief Remove the line from the application.
 *	<p>	If execute() followed by terminate() has not been called,
 *		this command cannot be undone (returns false).
 *
 * @return True, if the command is successfully undone.
 *
 */
bool DrawCommand::silent_undo() {
	if (!undoAble) return false;
	// remove drawings from back of pictures.
	for (int i = 0; i < drawable->size(); i++) {
		app->getUserPictures().pop_back();
	}
	return true;
}


/*! \brief Stop drawing the line, then push the line to the main application.
 * <p> If execute has not been called, this command dset redoAble and undoAble
 *		to false.
 *
 */
void DrawCommand::terminate() {
	active = false;
	// this function has not yet been executed, or has made no changes yet.
	if (!drawable) {
		undoAble = false;
		redoAble = false; // it will get discarded by App
		return;
	}

	// push changes to the window
	undoAble = true;
	redoAble = true;
	for (sf::VertexArray *pic : *drawable) {
		app->getUserPictures().push_back(pic);
	}
}

/*! \brief  This function is a to string method to be passed to network
 *
 * @return A C string of this Command
 */
const char* DrawCommand::parseOut() {
    std::string builder = "3 " + std::string(Command::parseOut());
    char * cstr = new char [builder.length()+1];
    std::strcpy (cstr, builder.c_str());
    return cstr;
};

/** \brief Adjust this command's fields to match the given input.
    e.g. parseIn = "0Generic Command Description\n000\0"
    e.g. parseIn = "0Generic Command Description\n000,50,50,255,255,255;25,25,255,255,255...."
 * @param [in] commandDescription The command as a parsed string.
 **/
DrawCommand* DrawCommand::parseIn(std::string input) {
    // remaining input must be of generic command
    Command::parseIn(input);
    return this;
};

#endif