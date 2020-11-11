
#ifndef COMMANDFILTER_CPP
#define COMMANDFILTER_CPP

#include "CommandFilter.hpp"

#include<iostream>

/** \brief Given a command, return it's corresponding message (if any).
*/
std::string CommandFilter::commandFilter(Command* command) {
	switch (command->commandType) {
		case GENERIC: return "";
		case CLOSE: return "";
		case COLOR: return "";
		case DRAW: return command->get_m_commandDescription();
		// case DRAW: return command->parseOut();
		case FILL: return command->get_m_commandDescription();
		case REDO: return command->get_m_commandDescription();
		case UNDO: return command->get_m_commandDescription();
		case CHAT: return command->get_m_commandDescription();
	default:
		throw "Unknown command type caught in command filter.";
	}
	return "";
};

#endif