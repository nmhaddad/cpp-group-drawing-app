/**
 *  @file   test4.cpp
 *  @brief  Unit test for checking that parsing functions output correctly.
 *  @author Mike and Team Five
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
#include "ColorCommand.hpp"
#include "CloseCommand.hpp"
#include "DrawCommand.hpp"
#include "FillCommand.hpp"
#include "RedoCommand.hpp"
#include "UndoCommand.hpp"
#include "ChatCommand.hpp"

/*! \brief 	test parsing output to see if they are correct
*
*/
TEST_CASE("ColorCommand parseOut Testing")
{
    App* app = new App();
    Command* colorCommand = new ColorCommand(app);
    REQUIRE(std::string(colorCommand->parseOut()).compare("") == 0);
    REQUIRE(std::string(colorCommand->parseOut()).compare("1 ") != 0);
}

TEST_CASE("CloseCommand parseOut Testing")
{
    App* app = new App();
    Command* closeCommand = new CloseCommand(app);
    REQUIRE(std::string(closeCommand->parseOut()).compare("") == 0);
    REQUIRE(std::string(closeCommand->parseOut()).compare("2 ") != 0);
}

TEST_CASE("DrawCommand parseOut Testing")
{
    App* app = new App();
    Command* drawCommand = new DrawCommand(app);
    REQUIRE(std::string(drawCommand->parseOut()).compare("3 0 0 ") == 0);
    REQUIRE(std::string(drawCommand->parseOut()).compare("3 1 0 ") != 0);
}

TEST_CASE("FillCommand parseOut Testing")
{
    App* app = new App();
    Command* fillCommand = new FillCommand(app);
    REQUIRE(std::string(fillCommand->parseOut()).compare("4 0 0 ") == 0);
    REQUIRE(std::string(fillCommand->parseOut()).compare("4 1 0 ") != 0);
}

TEST_CASE("RedoCommand parseOut Testing")
{
    App* app = new App();
    Command* redoCommand = new RedoCommand(app);
    REQUIRE(std::string(redoCommand->parseOut()).compare("5 0 0 ") == 0);
    REQUIRE(std::string(redoCommand->parseOut()).compare("5 1 0 ") != 0);
}

TEST_CASE("UndoCommand parseOut Testing")
{
    App* app = new App();
    Command* undoCommand = new UndoCommand(app);
    REQUIRE(std::string(undoCommand->parseOut()).compare("6 0 0 ") == 0);
    REQUIRE(std::string(undoCommand->parseOut()).compare("6 1 0 ") != 0);
}


TEST_CASE("ColorCommand parseIn Testing")
{
    App* app = new App();
    Command* colorCommand = new ColorCommand(app);
    Command* c;

    // ColorCommand parseIn
    c = colorCommand->parseIn("1 0 0 ");
    REQUIRE(c == colorCommand);
}

TEST_CASE("CloseCommand parseIn Testing")
{
    App* app = new App();
    Command* closeCommand = new CloseCommand(app);
    Command* c;

    // CloseCommand parseIn
    c = closeCommand->parseIn("1 0 0 ");
    REQUIRE(c == closeCommand);
}

TEST_CASE("DrawCommand parseIn Testing")
{
    App* app = new App();
    Command* drawCommand = new DrawCommand(app);
    Command* drawCommandAlt = new DrawCommand(app);
    Command* c;

    // DrawCommand parseIn
    c = drawCommand->parseIn("3 0 0 ");
    REQUIRE(c == drawCommand);
    c = drawCommandAlt->parseIn("3 1 0 ");
    REQUIRE(c != drawCommand);
}

TEST_CASE("FillCommand parseIn Testing")
{
    App *app = new App();
    Command *fillCommand = new FillCommand(app);
    Command *fillCommandAlt = new FillCommand(app);
    Command* c;

    // FillCommand parseIn
    c = fillCommand->parseIn("4 0 0 ");
    REQUIRE(c == fillCommand);
    c = fillCommandAlt->parseIn("4 1 0 ");
    REQUIRE(c != fillCommand);
}

TEST_CASE("RedoCommand parseIn Testing")
{
    App* app = new App();
    Command* redoCommand = new RedoCommand(app);
    Command* redoCommandAlt = new RedoCommand(app);
    Command* c;

    // RedoCommand parseIn
    c = redoCommand->parseIn("6 0 0 ");
    REQUIRE(c == redoCommand);
    c = redoCommandAlt->parseIn("6 1 0 ");
    REQUIRE(c != redoCommand);
}

TEST_CASE("UndoCommand parseIn Testing")
{
    App* app = new App();
    Command* undoCommand = new UndoCommand(app);
    Command* undoCommandAlt = new UndoCommand(app);
    Command* c;

    // UndoCommand parseIn
    c = undoCommand->parseIn("5 0 0 ");
    REQUIRE(c == undoCommand);
    c = undoCommandAlt->parseIn("5 1 0 ");
    REQUIRE(c != undoCommand);
}

TEST_CASE("ChatCommand New Feature testing")
{
    App* app = new App();
    Command* chatCommand = new ChatCommand(app);
    Command* chatCommandAlt = new ChatCommand(app);
    Command* c;

    // UndoCommand parseIn
    c = chatCommand->parseIn("7 hello ");
    REQUIRE(c == chatCommand);
}