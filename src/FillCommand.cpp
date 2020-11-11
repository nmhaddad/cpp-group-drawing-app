/**
 *  @file   FillCommand.cpp
 *  @brief  Implementation of FillCommand.hpp
 *  @author John
 *  @date   2020-02-09
 ***********************************************/
#ifndef FILLCOMMAND_CPP
#define FILLCOMMAND_CPP

//SFML libraries
#include <SFML/Graphics.hpp>
#include <cstring>
//Local libraries
#include "FillCommand.hpp"


 /*! \brief Fill the application with the current color.
  * <p>	Subsequent calls will write to the previously filled space;
  *		a larger window will not be entirely filled.
  *
  * @return True, if the command is successfully run.
  *
  */
bool FillCommand::silent_execute() {
	if (redoAble) {
		// push changes to app.
		app->getUserPictures().push_back(drawable->back());
		return true;
	}

	// instantiate new picture list
	drawable = new std::vector<sf::VertexArray*>();

	// instantiate new picture
	drawable->push_back(new sf::VertexArray(sf::Quads, 0));

	// create the rectangle
  drawable->back()->append(sf::Vertex(sf::Vector2f((float)0, (float)0), *app->color));
  drawable->back()->append(sf::Vertex(sf::Vector2f(sf::Vector2i(app->GetWindow().getSize().x, 0)), *app->color));
  drawable->back()->append(sf::Vertex(sf::Vector2f(sf::Vector2i(app->GetWindow().getSize().x, app->GetWindow().getSize().y)), *app->color));
  drawable->back()->append(sf::Vertex(sf::Vector2f(sf::Vector2i(0, app->GetWindow().getSize().y)), *app->color));
	// push changes to app.
	app->getUserPictures().push_back(drawable->back());
	redoAble = true;
	undoAble = true;



	return true;
};


/*! \brief Revert the background fill.
 *
 * @return True if the command is successfully undone.
 *
 */
bool FillCommand::silent_undo() {
	if (app->getUserPictures().size() == 0) return false;
	if (undoAble == false) return false;
	app->getUserPictures().pop_back();
	return true;
};

/*! \brief  This function is a to string method to be passed to network
 *
 * @return A C string of this Command
 */
const char* FillCommand::parseOut() {
    std::string builder = "4 " + std::string(Command::parseOut());
    char * cstr = new char [builder.length()+1];
    std::strcpy (cstr, builder.c_str());
    return cstr;
};

/** \brief Adjust this command's fields to match the given input.
    e.g. parseIn = "0Generic Command Description\n000\0"
    e.g. parseIn = "0Generic Command Description\n000,50,50,255,255,255;25,25,255,255,255...."
 * @param [in] commandDescription The command as a parsed string.
 **/
FillCommand* FillCommand::parseIn(std::string input) {
    // remaining input must be of generic command
    Command::parseIn(input);
    return this;
};

#endif