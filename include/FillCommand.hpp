/**
 *  @file   FillCommand.hpp
 *  @brief  User command to fill the screen with one color.
 *  @author John
 *  @date   2020-02-09
 ***********************************************/
#ifndef FILLCOMMAND_HPP
#define FILLCOMMAND_HPP

// SFML libraries
#include <SFML/Graphics.hpp>
// Project libraries
#include "App.hpp"
#include "Command.hpp"

/** \brief Fill the screen with one color.
*/
class FillCommand : public Command {
private:
	bool silent_execute() override;
	bool silent_undo() override;
	FillCommand();
public:

    /*! \brief  Create a new fill command.
     *
     * @return a new fill command.
     */
	FillCommand(App* app) : Command("FC", app) {commandType = FILL; };

    FillCommand* parseIn(std::string input) override;

    const char* parseOut() override;
};


#endif