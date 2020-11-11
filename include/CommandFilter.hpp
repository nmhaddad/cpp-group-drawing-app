/**
 *  @file   CommandFilter.hpp
 *  @brief  Handles filtering commands to messages.
 *  @author John
 *  @date   2020-04-04
 ***********************************************/
#ifndef COMMANDFILTER_HPP
#define COMMANDFILTER_HPP

 // Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Include standard library C++ libraries.
#include <queue>
#include <stack>
// Project header files
#include "Command.hpp"

 /** \brief Handles filtering commands for sending and recieving.
 */
class CommandFilter {
public:
    static std::string commandFilter(Command* command);
};

#endif