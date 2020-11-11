/**
 *  @file   InputHandler.hpp
 *  @brief  Given events in the program, delegates them to commands.
 *  @author John
 *  @date   2020-02-09
 ***********************************************/
#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

 //SFML libraries
#include <SFML/Graphics.hpp>
// Include standard library C++ libraries.
#include <iostream>
//Local libraries
#include "Command.hpp"
#include "DrawCommand.hpp"
#include "CloseCommand.hpp"
#include "ColorCommand.hpp"
#include "FillCommand.hpp"
#include "UndoCommand.hpp"
#include "RedoCommand.hpp"
#include "ChatCommand.hpp"

/** \brief Handle input events.
*/
class InputHandler {
public:
    enum colorEnum{RED, BLACK, GREEN, BLUE, YELLOW, RAINBOW, NONE };
    static int op;
	static Command* event(const sf::Event &event, App* app);
};

#endif