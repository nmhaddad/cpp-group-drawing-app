/**
 *  @file   UndoCommand.hpp
 *  @brief  User command to undo the last command.
 *  @author John
 *  @date   2020-02-09
 ***********************************************/
#ifndef UNDOCOMMAND_HPP
#define UNDOCOMMAND_HPP

 // Include our Third-Party SFML header
 // #include ...
 // Include standard library C++ libraries.
#include <string>
// Project header files
#include "App.hpp"
#include "Command.hpp"

/** \brief Un-execute the last command.
*/
class UndoCommand : public Command {
private:
	bool silent_execute() override;
	bool silent_undo() override;
	UndoCommand();
public:

    /*! \brief  Create a new undo command.
     *
     * @return a new undo command.
     */
	UndoCommand(App* app) : Command("UC", app) {commandType = UNDO; };

    UndoCommand* parseIn(std::string input) override;

    const char* parseOut() override;
};

#endif