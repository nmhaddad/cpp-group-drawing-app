/**
 *  @file   CloseCommand.hpp
 *  @brief  User command to close the program.
 *  @author John
 *  @date   2020-02-09
 ***********************************************/
#ifndef CLOSECOMMAND_HPP
#define CLOSECOMMAND_HPP

// Project header files
#include "App.hpp"
#include "Command.hpp"

/** \brief Close the program.
*/
class CloseCommand : public Command {
private:
	bool silent_execute() override;
	bool silent_undo() override;
	CloseCommand();
public:

    /*! \brief  Create a new close command (tells the console to close the window and end the program.
     *
     * @return a new close command.
     */
	CloseCommand(App* app) : Command("XC", app) {commandType = CLOSE; };

    CloseCommand* parseIn(std::string input) override;
    const char* parseOut() override;
};

#endif
