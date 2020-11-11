/**
 *  @file   InputHandler.cpp
 *  @brief  Implementation of InputHandler.hpp
 *  @author John
 *  @date   2020-02-09
 ***********************************************/
#ifndef INPUTHANDLER_CPP
#define INPUTHANDLER_CPP

// SFML libraries
#include <SFML/Graphics.hpp>
// Include standard library C++ libraries.
#include <iostream>
// Local libraries
#include "InputHandler.hpp"
#include "App.hpp"
#include "DrawCommand.hpp"
#include "RainbowDrawCommand.hpp"
#include "CloseCommand.hpp"
#include "ColorCommand.hpp"
#include "FillCommand.hpp"
#include "UndoCommand.hpp"
#include "RedoCommand.hpp"
#include "ChatCommand.hpp"


int InputHandler::op = InputHandler::NONE;

bool usingRainBow = false;

/*! \brief Handle a given event or return a command that can.
 * <p> Informs all active commands of events, so they can decide if
 *	   they should remain active or otherwise update their states.
 * <p> Updates the mouse position when the mouse is moved.
 * <p> Checks keyPresses and and mouse presses; generates and returns new, corresponding commands.
 *
 * @return Null or a command that executes as a consequence of the given event.
 *
 */
Command* InputHandler::event(const sf::Event &event, App* app) {
	for (Command * command : app->getActiveCommands()) {
		command->inform(event);
	}
	if (event.type == sf::Event::MouseMoved) {
		app->mouseX = sf::Mouse::getPosition(app->GetWindow()).x;
		app->mouseY = sf::Mouse::getPosition(app->GetWindow()).y;
		return NULL;
	}
	if (event.type == sf::Event::Closed) return new CloseCommand(app);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape) return new CloseCommand(app);
	if (event.type == sf::Event::KeyPressed) {
		// preemtively set op to none.  if no new color is selected, set it to it's old value.
		int old_op = op;
		bool old_usingRainBow = usingRainBow;
		op = NONE;
		usingRainBow = false;
		if (event.key.code == sf::Keyboard::Key::Num1) return new ColorCommand(&sf::Color::Black, "Black", app);
		if (event.key.code == sf::Keyboard::Key::Num2) return new ColorCommand(&sf::Color::White, "White", app);
		if (event.key.code == sf::Keyboard::Key::Num3) return new ColorCommand(&sf::Color::Red, "Red", app);
		if (event.key.code == sf::Keyboard::Key::Num4) return new ColorCommand(&sf::Color::Green, "Green", app);
		if (event.key.code == sf::Keyboard::Key::Num5) return new ColorCommand(&sf::Color::Blue, "Blue", app);
		if (event.key.code == sf::Keyboard::Key::Num6) return new ColorCommand(&sf::Color::Yellow, "Yellow", app);
		if (event.key.code == sf::Keyboard::Key::Num7) return new ColorCommand(&sf::Color::Magenta, "Magenta", app);
		if (event.key.code == sf::Keyboard::Key::Num8) return new ColorCommand(&sf::Color::Cyan, "Cyan", app);
		op = old_op;
		usingRainBow = old_usingRainBow;
		if (event.key.code == sf::Keyboard::Key::Space) return new FillCommand(app);
		if (event.key.code == sf::Keyboard::Key::Z) return new UndoCommand(app);
		if (event.key.code == sf::Keyboard::Key::Y) return new RedoCommand(app);
		if (event.key.code == sf::Keyboard::Key::C) return new ChatCommand(app);
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (app->mouseX > 0
			&& app->mouseY > 0
			&& app->mouseX < app->GetWindow().getSize().x
			&& app->mouseY < app->GetWindow().getSize().y) {
			if (usingRainBow) {
				return new RainbowDrawCommand(app);
			}
			else {
				return new DrawCommand(app);
			}
		}
	}
	return NULL;
};

#endif
