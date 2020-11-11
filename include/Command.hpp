/** 
 *  @file   Command.hpp 
 *  @brief  Represents an actionable command by the user.
 *  @author Mike and John
 *  @date   2020-02-09 
 ***********************************************/
#ifndef COMMAND_HPP
#define COMMAND_HPP

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
// Include standard library C++ libraries.
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
// Project header files
class App; // forward declaration of App.

// class generic enum
enum CommandEnum { GENERIC, CLOSE, COLOR, DRAW, FILL, REDO, UNDO, CHAT, RAINBOWDRAW };

/** \brief abstract command class; children are instantiated by InputHandler and managed by App.
*/
class Command {
private:
	std::string m_commandDescription;	//!< The description of the command.
protected:
	// Member variables
	bool undoAble = false;	//!< True if the command can be undone; can change with the state of the command.
	bool redoAble = false;	//!< True if the command can be redone; can change with the state of the command.
	bool active = false;	//!< True if the command is still active; can change with the state of the command.
	bool is_server_command = false;
	std::vector<sf::VertexArray*> *drawable = nullptr;	//!< What has been drawn with the command so far, or nullptr.
	App* app = nullptr;
	// Member methods
	/** \brief Silently does the function.
	 * @return True, if the function was done. */
	virtual bool silent_execute() = 0;
	/** \brief Silently undoes the function.
	 * @return True, if the function was undone. */
	virtual bool silent_undo() = 0;

public:
	/** \brief Each command also takes in a string name. While this may not be optimal,
	 *	it allows us to easily debug what each command is doing in a textual form.
	 * @param [in] commandDescription The description of the command. */
  	int commandType;

    /*! \brief  Create a new command.
     *
     * @return a new command.
     */
	Command(const std::string &commandDescription, App* app) : m_commandDescription(commandDescription), app(app), commandType(GENERIC) {}

	// Destructor for a command
	virtual ~Command();
	bool execute();
	bool undo();
	virtual void terminate();
	virtual void inform(sf::Event event);

	// Accessor methods
	/** \brief Accessor for <code>drawable</code>.
	 * @return <code>drawable</code> */
	const std::vector<sf::VertexArray*> * getDrawable() { return drawable; };
	/** \brief Accessor for <code>m_commandDescription</code>.
	 * @return <code>m_commandDescription</code> */
	const std::string get_m_commandDescription() { return m_commandDescription; };
	/** \brief Accessor for <code>undoAble</code>.
	 * @return <code>undoAble</code> */
	bool isUndoAble() { return undoAble; };
	/** \brief Accessor for <code>redoAble</code>.
	 * @return <code>redoAble</code> */
	bool isRedoAble() { return redoAble; };
	/** \brief Accessor for <code>active</code>.
	 * @return <code>active</code> */
	bool isActive() { return active; };


	/** \brief Adjust this command's fields to match the given input.
	 *	e.g. parseIn = "0Generic Command Description\n000\0"
	 *	e.g. parseIn = "0Generic Command Description\n000,50,50,255,255,255;25,25,255,255,255...."
	 *
	 * @param [in] commandDescription The command as a parsed string.
	 **/
    virtual Command* parseIn(std::string input);

    /** \brief Convert contents to readable, parsable char*.
     *   e.g. parseIn = "0Generic Command Description\n000,50,50,255,255,255;25,25,255,255,255...."
     *   "<0 for generic command><command description>\n<undoable 0/1>..."
     *   if parseOut is called by a child, that child will handle it's own data, then append this parent call to it's butt.
     *   "<some child command, e.g. 1><that command's junk>...<0 for generic command><command description>\n<undoable 0/1>..."
	 *
     * @return the command as a parsed string
     * */
    virtual const char* parseOut();


};

#endif
