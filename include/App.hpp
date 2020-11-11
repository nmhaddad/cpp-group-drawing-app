/** 
 *  @file   App.hpp 
 *  @brief  App class interface
 *  @author Mike and John
 *  @date   2020-02-09 
 ***********************************************/
#ifndef APP_HPP
#define APP_HPP

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Include standard library C++ libraries.
#include <cassert>
#include <queue>
#include <stack>
// Project header files
class Command; // forward declaration

/** \brief Our Application called 'App'.
*/
class App {
private:
// Member variables
	std::stack<Command*> m_commands = std::stack<Command*>();	//!< Do-deck; Queue stores the next command to do.
	std::vector<Command*> m_active = std::vector<Command*>();	//!< Active-deck; Queue stores any commands that didn't immediately resolve.
	std::stack<Command*> m_undo = std::stack<Command*>();	//!< Undo-deck; Stack that stores the last action to occur.
	std::vector<Command*> completedCommands = std::vector<Command*>();	//!< Stores active commands that have been finished
	sf::Image* m_image = new sf::Image;	//!< Main image
	sf::Sprite* m_sprite = new sf::Sprite;//!< Create a sprite that we overlay on top of the texture.
	sf::Texture* m_texture = new sf::Texture;	//!< Texture sent to the GPU for rendering
	std::vector<sf::VertexArray*> *userPictures = new std::vector<sf::VertexArray*>();	//!< Lines or changes made by the user's previous, terminated commands.

// Member functions
	// Store the address of our funcion pointer
	// for each of the callback functions.
	void (*m_initFunc)(void) = nullptr;	//!< Callback address of main's init function.
	bool (*m_updateFunc)(void) = nullptr;	//!< Callback address of main's update function.
	void (*m_drawFunc)(void) = nullptr;	//!< Callback address of main's draw function.
public:
// Member Variables	
	// Keeps track of the previous mouse and current mouse positions
	// 'pmouse' is where the mouse previously was.
	// 'mouse' records where the mouse currently is.
	int pmouseX = 0, //!< The previous x position of the mouse.
		pmouseY = 0, //!< The previous y position of the mouse.
		mouseX = 0, //!< The current x position of the mouse.
		mouseY = 0;	//!< The current y position of the mouse.
	sf::RenderWindow* m_window = nullptr;	//!< Themain rendering window.
	sf::Color* color = new sf::Color();	//!< The application's current color.
	std::string connectingClientName; //!< The connecting client's given name.
// Member functions
	void	ExecuteCommand();
	void	ExecuteActiveCommands();
	void	TerminateActiveCommands();
	void	UndoExecutedCommand();
// Accessors
	void AddCommand(Command* command);
	const std::stack<Command*> getFutureCommands();
	const std::vector<Command*> getActiveCommands();
	const std::stack<Command*> getPastCommands();
	Command* getCompletedCommands();
	sf::Image& GetImage();
	sf::Sprite& GetSprite();
	sf::Texture& GetTexture();
	sf::RenderWindow& GetWindow();
	std::vector<sf::VertexArray*>& getUserPictures();

// Core Methods
	void Destroy();
	void Init(void (*initFunction)(void));
	void UpdateCallback(bool (*updateFunction)(void));
	void DrawCallback(void (*drawFunction)(void));
	void Loop();
	void terminate();

};


#endif
