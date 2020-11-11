/** 
 *  @file   App.cpp 
 *  @brief  Main class for program
 *  @author Mike and John
 *  @date   2020-02-09 
 ***********************************************/
#ifndef APP_CPP
#define APP_CPP

#include "App.hpp"
#include "Command.hpp" // resolve forward declaration


/*! \brief 	Given a command, place it on the to-do stack.
 *
 * @param[in] command Command to place on stack.
 *
 */
void App::AddCommand(Command* command){
	m_commands.push(command);
}


/*! \brief 	Execute the next to-do command.
 * <p> If there is a next command, execute it; depending on
 *	the commands state after execution, place it on the
 *  active stack, the undo stack, or discard it.
 *
 */
void App::ExecuteCommand(){
	// are there actually any commands to complete?
	if (m_commands.size() == 0) { return; }
	// retrieve the next command and execute it
	Command* command = m_commands.top();
	// all commands are removed from the future command stream once they've been completed
	// remove before execution: it's execution assumes it isn't the command stacks
	m_commands.pop();
	command->execute();
	// the command will tell us it's current state.
	if (command->isActive()) {
		// if the command is active, it should be moved to the active pile.
#ifdef _DEBUG
		std::cout << "Active: " << command << ' ' << command->get_m_commandDescription() << std::endl;
#endif
		m_active.push_back(command);
	} else if (command->isUndoAble()) {
		// the command is inactive. if it is undoable, move it to the undo pile.
#ifdef _DEBUG
		std::cout << "From future to undo stack: " << command->get_m_commandDescription() << std::endl;
#endif
		m_undo.push(command);
	}
}


/*! \brief 	If there are any active commands, execute them.
 *	Commands will self flag as being complete; if they're done,
 *  move them to the appropriate pile.
 *
 */
void App::ExecuteActiveCommands() {
	// are there actually any commands to complete?
	if (m_active.size() == 0) { return; }

	// execute each command once.
	for (Command* command : m_active) {
		// double check that the command hasn't been preemtively terminated first.
		if (command->isActive()) {
			command->execute();
		}
	}

	// check each command for continued activity
    std::vector<Command*> to_delete;
	for (size_t i = 0; i < m_active.size(); i++) {
		Command *command = m_active.at(i);
		if (!command->isActive()) {
			// command i is no longer active.
#ifdef _DEBUG
            std::cout << command->get_m_commandDescription() << ' ' << i << " is no longer active." << std::endl;
#endif
            // add it to the completedCommands stack, to inform external modules of it's completion
            completedCommands.push_back(command);

            //queue for deletion
            to_delete.push_back(command);

			if (command->isUndoAble()) {
				// the command is inactive. if it is undoable, move it to the undo pile.
#ifdef _DEBUG
				std::cout << "From active to undo stack: " << command->get_m_commandDescription() << std::endl;
#endif
				m_undo.push(command);
			}
		}
	}
    for (Command* c_to_delete : to_delete) {
        // remove it from the active pile.
        size_t i = 0;
        for (; i < m_active.size(); i++) {
            if (m_active[i] == c_to_delete) break;
        }
        m_active.erase(m_active.begin() + i);
    }
}


/*! \brief 	Undo the most recent command.
 * <p> Given that there is a command on the Undo stack,
 *   check if it can be undone; if not, do nothing. 
 *   Currently, this will never happen: only un-doable commands
 *   are placed on the unto stack.  Otherwise,
 *   undo it, then place it on the to-do execution stack.
 *   There are currently no commands that will revert to active
 *   upon being undone.
 *	<p>	Undo safety is ensured using four stacks:
 *	<p>	- <code>m_commands</code>, which is
 *		commands that have not yet been executed; new commands are pushed
 *		to this stack before being immediately executed and removed.
 *	<p>	- <code>m_active</code>,
 *		which holds commands whose execution is ongoing; these commands may have internal,
 *		partially complete objects.
 *	<p>	- <code>m_undo</code>, which holds commands that have
 *		been executed and indicate that they can be undone or are blocking attempts to
 *		undo them to protect data integrity.
 *	<p>	- Finally, <code>*userPictures</code> holds all previously rendered pictures; commands
 *		that have undo enabled can restore this to the same state that it was in before they were executed
 *		by removing items off the top of it.  Thus, the top of this stack should always have the
 *		pictures rendered by the command at the top of <code>m_undo</code>.
 *
 */
void App::UndoExecutedCommand() {
	// are there actually any commands to complete?
	if (m_undo.size() == 0) {

#ifdef _DEBUG
		std::cout << "No commands to undo" << std::endl; 
#endif
		return; 
	}
	// retrieve the next command and execute it
	Command* command = m_undo.top();
	if (!command->isUndoAble()) {
		// can this command be undone?
#ifdef _DEBUG
		std::cout << "Not undo-able: " << command->get_m_commandDescription() << std::endl;
#endif
		return;
	}
	// remove from the undo stack
	m_undo.pop();
	command->undo();
	// put it on the to-do stack
#ifdef _DEBUG
	std::cout << "From undo to future stack: " << command->get_m_commandDescription() << std::endl;
#endif
	m_commands.push(command);
}


/*! \brief 	Forcibly clear the active commands.
 * <p> Terminate all currently active commands, then
 *   call <code>ExecuteActiveCommands()</code> to clear
 *   them from the active stack.
 *
 */
void App::TerminateActiveCommands() {
#ifdef _DEBUG
    std::cout << "\nFORCE CLOSING ALL ACTIVE COMMANDS.\n";
#endif
	for (Command* command : m_active) {
		command->terminate();
	}
	ExecuteActiveCommands();
	if (m_active.size() != 0) throw new std::domain_error("A command failed to terminate properly.");
}


/*! \brief 	Return a constant reference to m_commands, so that
 *		we do not have to publicly expose it.
 *
 * @return The vector of future commands in the order they are to be executed.
 *
 */
const std::stack<Command*> App::getFutureCommands() {
	return m_commands;
}


/*! \brief 	Return a constant reference to m_active, so that
 *		we do not have to publicly expose it.
 *
 * @return The vector of active commands in no particular order.
 * 
 */
const std::vector<Command*> App::getActiveCommands() {
	return m_active;
}


/*! \brief 	Return a constant reference to m_undo, so that
 *		we do not have to publicly expose it.
 *
 * @return The vector of past commands in the order they are to be executed.
 *
 */
 const std::stack<Command*> App::getPastCommands() {
	return m_undo;
}


/*! \brief 	Return a reference to our m_image, so that
 *		we do not have to publicly expose it.
 *
 * @return A reference to m_image.
 *
 */
sf::Image& App::GetImage(){
	return *m_image;
}


/*! \brief 	Return a reference to our m_Sprite so that
 *		we do not have to publicly expose it.
 *
 * @return A reference to m_sprite.
 * 
 */
sf::Sprite& App::GetSprite() {
	return *m_sprite;
}


/*! \brief 	Return a reference to our m_texture so that
 *		we do not have to publicly expose it.
 *
 * @return A reference to m_texture.
 * 
 */
sf::Texture& App::GetTexture(){
	return *m_texture;
}


/*! \brief 	Return a reference to our m_window so that we
 *		do not have to publicly expose it.
 *
 * @return A reference to m_window.
 */
sf::RenderWindow& App::GetWindow(){
	return *m_window;
}


/*! \brief 	Return a reference to our userPictures so that we
 *		do not have to publicly expose it.
 *
 * @return A reference to userPictures.
 * 
*/
std::vector<sf::VertexArray*>& App::getUserPictures() {
	return *userPictures;
}


/*! \brief 	A destroy function we manually call at end of our program.
 *	<p> I did not expand this to encapsulate new variables since C++'s
 *	default destructors should be capable of handling deconstruction.
 *
 */
void App::Destroy(){
	delete m_image;
	delete m_sprite;
	delete m_texture;
}


/*! \brief 	Initializes the App and sets up the main
 *		rendering window(i.e. our canvas.)
 *
 */
void App::Init(void (*initFunction)(void)){
	// Create our window
	m_window = new sf::RenderWindow(sf::VideoMode(600,400),"Mini-Paint alpha 0.0.2",sf::Style::Titlebar);
	m_window->setVerticalSyncEnabled(true);
	// Create an image which stores the pixels we will update
	m_image->create(600,400,sf::Color::White);
	assert(m_image != nullptr && "m_image != nullptr");
	// Create a texture which lives in the GPU and will render our image
	m_texture->loadFromImage(*m_image);
	assert(m_texture != nullptr && "m_texture != nullptr");
	// Create a sprite which is the entity that can be textured
	m_sprite->setTexture(*m_texture);
	assert(m_sprite != nullptr && "m_sprite != nullptr");
	// Set our initialization function to perform any user
	// initialization
	m_initFunc = initFunction;

}


/*! \brief 	Set a callback function which will be called
 *		each iteration of the main loop before drawing.
 *
 * @param[in] updateFunction Called before drawing.
 *
 */
void App::UpdateCallback(bool (*updateFunction)(void)){
	m_updateFunc = updateFunction;
}


/*! \brief 	Set a callback function which will be called
 *		each iteration of the main loop after update.
 *
 * @param[in] drawFunction Called to draw the window contents.
 *
*/
void App::DrawCallback(void (*drawFunction)(void)){
	m_drawFunc = drawFunction;
}


/*! \brief  If an active command has been completed, return it.
 *
 * If multiple active commands have been completed, return the oldest first.
 * Additional calls will return progressively newer commands.
 *
 * If no commands remain to be returned, return nullptr.
 *
 * Disambiguation: This function does not return ALL commands that are done.
 *                 ONLY active commands get returned here.
 */
Command* App::getCompletedCommands() {
    if (completedCommands.size() > 0) {
        Command* c = completedCommands.front();
        completedCommands.erase(completedCommands.begin());
        return c;
    }
    return nullptr;
}



/*! \brief  Terminate the app on its next iteration.
 *
 */
void App::terminate() {
    m_window->close(); // this will be automatically interpreted as a request to close the app
}


/*! \brief 	The main loop function which handles initialization
 *		and will be executed until the main window is closed.
 *		Within the loop function the update and draw callback
 *		functions will be called.
 *
 */
void App::Loop() {
	// Call the init function
	m_initFunc();

	// Start the main rendering loop
	while(m_window->isOpen()){
		// Clear the window
		m_window->clear();
		// Updates specified by the user
		bool update_okay = m_updateFunc();
		if (!update_okay && !m_window->isOpen()) {
			break;
		}
		// Update the texture
		// Additional drawing specified by user
		m_drawFunc();

		// Display the canvas
		m_window->display();
	}
}


#endif