/**
 *  @file   extraFeatureTest.cpp
 *  @brief  Unit test for testing the chat feature.
 *  @author Duncan and Team Five
 *  @date   2020-9-04
 * referenced https://medium.com/@junaidrahim/a-guide-to-using-catch2-for-unit-testing-in-c-f0f5450d05fb
 ***********************************************/
//#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Include standard library C++ libraries.
#include <iostream>
#include <string>
// Project header files
#include "App.hpp"
#include "Command.hpp"
#include "ChatCommand.hpp"


/*! \brief 	test parsing output to see if they are correct
*
*/
TEST_CASE("Testing ChatCommand")
{
    App* app = new App();
    Command* chatCommand = new ChatCommand(app);
    REQUIRE(std::string(chatCommand->parseOut()).compare("7 /*/*/") == 0);
    REQUIRE(std::string(chatCommand->parseOut()).compare("7/*/*/ ") != 0);
    std::cout << "extraFeatureTest 1 passed" << std::endl;
}

TEST_CASE("Testing ChatCommand input")
{
    App* app = new App();
    Command* chatCommand = new ChatCommand(app);
    REQUIRE(std::string(chatCommand->parseIn("hello")->parseOut()).compare("7 hello /*/*/") != 0);
    REQUIRE(std::string(chatCommand->parseIn("hello")->parseOut()).compare("7hello/*/*/ ") != 0);
    std::cout << "extraFeatureTest 2 passed" << std::endl;
}