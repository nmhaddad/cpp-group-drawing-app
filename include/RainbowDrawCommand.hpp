/**
 *  @file   RainbowDrawCommand.hpp
 *  @brief  RainbowDrawCommand implementation, all drawing actions are commands.
 *  @author John
 *  @date   2020-04-14
 ***********************************************/
#ifndef RAINBOWDRAWCOMMAND_H
#define RAINBOWDRAWCOMMAND_H

 // Include our Third-Party SFML header
 // #include ...
 // Include standard library C++ libraries.
#include <string>
// Project libraries
#include "DrawCommand.hpp"


enum ColorEnum{RED, YELLOW, GREEN, BLUE, PURPLE};

/** \brief draw a continuous line.
 *
 */
class RainbowDrawCommand : public DrawCommand {
private:
    bool supportCommand = false;    //!< is this the support version of the command?
    RainbowDrawCommand* shadow = nullptr;   //!< point to either the support command (or the parent command, if this is the support command)
    int tgtColor = RED;
    sf::Color toColor = sf::Color(255,0,0);
    RainbowDrawCommand();
protected:
    bool silent_execute() override;
    bool silent_undo() override;

    RainbowDrawCommand* getShadow();
public:

    /*! \brief  Create a new rainbow draw command.
     *
     * @return a new rainbow draw command.
     */
    RainbowDrawCommand(App* app) : DrawCommand(app, "R") {
        commandType = RAINBOWDRAW;
    };

    /*! \brief  Create a new rainbow draw command with a given description.
     *
     * @return a new rainbow draw command.
     */
    RainbowDrawCommand(App* app, const std::string &desc) : DrawCommand(app, desc+"R") {
        commandType = RAINBOWDRAW;
    };
    static bool colorStep(sf::Color* color, const sf::Color* toColor);
    bool cycleDrawableColor();

    RainbowDrawCommand* parseIn(std::string input) override;

    void terminate() override;

    const char* parseOut() override;
};

#endif
