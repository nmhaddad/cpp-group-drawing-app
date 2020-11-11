/**
 *  @file   RedoCommand.hpp
 *  @brief  User command to redo the last command.
 *  @author John
 *  @date   2020-02-09
 ***********************************************/
#ifndef REDOCOMMAND_HPP
#define REDOCOMMAND_HPP

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
#include <string>
// Project header files
#include "App.hpp"
#include "Command.hpp"

/** \brief Execute the last command.
*/
class RedoCommand : public Command {
private:
	/** \brief Silently execute the last command.
	 *	@return true */
	bool silent_execute() override;
	/** \brief Do nothing.
	 *	@return false */
	bool silent_undo() override;
	RedoCommand();
public:
    /*! \brief  Create a new redo command.
     *
     * @return a new redo command.
     */
	RedoCommand(App* app) : Command("RC", app) {commandType = REDO; };

    /*! \brief  Create a new redo command with a given string description.
     *
     * @return a new redo command.
     */
	RedoCommand* parseIn(std::string input) override;

    const char* parseOut() override;
};

#endif