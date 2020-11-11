/**
 *  @file   ColorCommand.hpp
 *  @brief  User command to set the color.
 *  @author John
 *  @date   2020-02-09
 ***********************************************/
#ifndef COLORCOMMAND_HPP
#define COLORCOMMAND_HPP

//SFML libraries
#include <SFML/Graphics.hpp>
//Local libraries
#include "App.hpp"
#include "Command.hpp"

/** \brief Set the current color.
*/
class ColorCommand : public Command {
private:
	const sf::Color* color;		//!< The new color.
	const sf::Color* lastColor; //!< The old color.

	bool silent_execute() override;
	bool silent_undo() override;
	ColorCommand();
public:
	/** \brief Adjust this command's fields to match the given input.
		e.g. parseIn = "0Generic Command Description\n000\0"
		e.g. parseIn = "0Generic Command Description\n000,50,50,255,255,255;25,25,255,255,255...."
	 * @param [in] commandDescription The command as a parsed string.
	 **/
	ColorCommand* parseIn(std::string input) override;

   const char* parseOut() override;

    /*! \brief  Create a new color command.
      *
      * @return a new color command.
      */
   ColorCommand(App* app) : Command("Color command: Generic Black.", app) {
        color = &sf::Color::Black;
        lastColor = &sf::Color::Black;
    };

    /*! \brief  Create a new color command with a color.
     *
     * @return a new color command.
     */
	ColorCommand(const sf::Color* color, const std::string &color_str, App* app)
		: Command("Color command: " + color_str
			+ " [r:" + std::to_string(color->r)
			+ ",g:" + std::to_string(color->g)
			+ ",b:" + std::to_string(color->b)
			+ ",a:" + std::to_string(color->a) + "]", app), color(color), lastColor(app->color)
	{
		// Note that undoAble is currently constructed to true,
		// meaning color commands can be un and redone.
		undoAble = false;
		commandType = COLOR;
	}; //!< construct color. edit undoAble at source level to tweak if colors can be undone.

};

#endif