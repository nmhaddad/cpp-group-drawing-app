/** 
 *  @file   Draw.hpp 
 *  @brief  Draw implementation, all drawing actions are commands. 
 *  @author Mike and John
 *  @date   2020-02-09 
 ***********************************************/
#ifndef DRAW_H
#define DRAW_H

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
#include <string>
// Project libraries
#include "App.hpp"
#include "Command.hpp"

/** \brief Draw a continuous line.
*/
class DrawCommand : public Command {
private:
protected:
	bool silent_execute() override;
	bool silent_undo() override;
	DrawCommand();
public:

    /*! \brief  Create a new draw command.
     *
     * @return a new draw command.
     */
	DrawCommand(App* app) : Command("DC", app) {commandType = DRAW;};

    /*! \brief  Create a new draw command with a specific command description.
     *
     * @return a new draw command.
     */
    DrawCommand(App* app, const std::string &description) : Command(description + "DC", app) { commandType = DRAW; };

    DrawCommand* parseIn(std::string input) override;

    void terminate() override;
	void inform(sf::Event event) override;

    const char* parseOut() override;
};

#endif
