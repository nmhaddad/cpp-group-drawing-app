/** 
 *  @file   App.cpp 
 *  @brief  Entry point into the program.
 *  @author Mike and John
 *  @date   2020-02-09 
 ***********************************************/

 // if active, will print debug statements
// include at compiler level with "-D_DEBUG" !
// #define _DEBUG 

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Network.hpp>
// Include standard library C++ libraries.
#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <type_traits>
#include <typeinfo>

// Project header files
#include "App.hpp"
#include "Command.hpp"
#include "InputHandler.hpp"
#include "UDPTool.hpp"
#include "CommandFilter.hpp"
#include "RainbowDrawCommand.hpp"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

// Include our Third-Party SFML header
// SFML
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

// NUKLEAR - for our GUI
#include <stdint.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SFML_GL2_IMPLEMENTATION

#include "nuklear.h"
#include "nuklear_sfml_gl2.h"

// main app variable
App app;

// client/server object
UDPTool *network;

// Gui variables
static sf::RenderWindow *gui_window;
struct nk_context *ctx;
struct nk_colorf bg;
sf::ContextSettings *settings;


Command *mainParseIn(std::string input) {
    if (input.size() == 0) return nullptr;
    if (input[0] == '0') return nullptr;
    if (input[0] == '1') return (new CloseCommand(&app))->parseIn(input);
    if (input[0] == '2') return (new ColorCommand(&app))->parseIn(input);
    if (input[0] == '3') return (new DrawCommand(&app))->parseIn(input);
    if (input[0] == '4') return (new FillCommand(&app))->parseIn(input);
    if (input[0] == '5') return (new RedoCommand(&app))->parseIn(input);
    if (input[0] == '6') return (new UndoCommand(&app))->parseIn(input);
    if (input[0] == '7') return (new ChatCommand(&app))->parseIn(input);
    if (input[0] == '8') return (new RainbowDrawCommand(&app))->parseIn(input);
    throw "Something's not right!!  Check mainParseIn.";
};


/*! \brief  Currently, do nothing.
 *	<p>	Call any initailization functions here.
 *		This might be for example setting up any
 *		global variables, allocating memory,
 *		dynamically loading any libraries, or
 *		doing nothing at all.
 *		
 */
void initialization(void) {
    std::cout << "Starting the App" << std::endl;
    // Setup the context
    settings = new sf::ContextSettings(24, 8, 4, 2, 2);

    /*  GUI
        We are creating a GUI context, and it needs
        to attach to our 'window'.
    */
    // Create a GUI window to draw to
    gui_window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "GUI Window", sf::Style::Default,
                                      *settings);
    gui_window->setVerticalSyncEnabled(true);
    gui_window->setActive(true);
    glViewport(0, 0, gui_window->getSize().x, gui_window->getSize().y);
    ctx = nk_sfml_init(gui_window);
    // Load Fonts: if none of these are loaded a default font will be used
    //Load Cursor: if you uncomment cursor loading please hide the cursor
    struct nk_font_atlas *atlas;
    nk_sfml_font_stash_begin(&atlas);
    nk_sfml_font_stash_end();
    // Setup a color for the nuklear gui
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;
}


extern bool usingRainBow;

/*
    GUI support function;
    handles button generation and operations.
*/
void doButton(struct nk_context *ctx, const char *name, int inputOp, const sf::Color *color) {
    if (nk_option_label(ctx, name, InputHandler::op == inputOp)) {
        if (InputHandler::op != inputOp) {
            InputHandler::op = inputOp;
            if (inputOp == InputHandler::RAINBOW) {
                usingRainBow = true;
                return;
            }
            usingRainBow = false;
            app.AddCommand(new ColorCommand(color, name, &app));
            app.ExecuteCommand();
        }
    }
};

// Function to render our GUI
void drawLayout(struct nk_context *ctx, struct nk_colorf &bg) {
    /* GUI */
    if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                 NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
        static int property = 20;
        nk_layout_row_dynamic(ctx, 30, 2);
        doButton(ctx, "Red", InputHandler::RED, &sf::Color::Red);
        doButton(ctx, "Black", InputHandler::BLACK, &sf::Color::Black);
        doButton(ctx, "Green", InputHandler::GREEN, &sf::Color::Green);
        doButton(ctx, "Blue", InputHandler::BLUE, &sf::Color::Blue);
        doButton(ctx, "Yellow", InputHandler::YELLOW, &sf::Color::Yellow);
        doButton(ctx, "Rainbow", InputHandler::RAINBOW, &sf::Color::Yellow);


        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "Pallette Picker:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 25, 1);
        if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
            nk_layout_row_dynamic(ctx, 120, 1);
            bg = nk_color_picker(ctx, bg, NK_RGBA);
            nk_layout_row_dynamic(ctx, 25, 1);
            bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
            bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
            bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
            bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
            nk_combo_end(ctx);

            // generate a new color command; send it to App.
            sf::Color *palletteColor(new sf::Color(0, 0, 0, 0));
            ColorCommand *palletteColorCommand(new ColorCommand(palletteColor, "pallette color", &app));
            palletteColor->r = (sf::Uint8)(bg.r * 255);
            palletteColor->g = (sf::Uint8)(bg.g * 255);
            palletteColor->b = (sf::Uint8)(bg.b * 255);
            palletteColor->a = (sf::Uint8)(bg.a * 255);
            app.AddCommand(palletteColorCommand);
            app.ExecuteCommand();
            InputHandler::op = InputHandler::NONE;
        }
    }
    nk_end(ctx);
}


/*! \brief  Update the application based on recent events and active commands.
 *	<p>	Passes each recent event to the input handler; if
 *		it is returned a new command, it executes it, then
 *		continues checking events.
 *	<p>	If there are active commands, it executes each.  Active commands are
 *		informed of new events in Input handler because it seemed right at the time
 *		and I'm too lazy to change it.
 *	<p>	Lastly, it updates the previous mouse position, which may have been changed in
 *		InputHandler due to a mouseMoved event.
 *		
 */
bool update(void) {
    // recieve and execute network events
    Command *c = nullptr;

    do {
        c = mainParseIn(network->recieve());

        if (c) {
            //      This is where we might handle command ordering using undo's and redo's
            //      to move younger commands out of the way, placing the old recieved command
            //      into the stack, and then re-doing the young undo commands, increasing their
            //      age counter by 1 (age counter is only handled on the server for now)
            app.AddCommand(c);
            app.ExecuteCommand();
        }
    } while (c);

    // capture and process user generated events
    sf::Event event;
    while (app.m_window->pollEvent(event)) {
        Command *myCommand = InputHandler::event(event, &app);
        if (myCommand) {
            app.AddCommand(myCommand);
            app.ExecuteCommand();
            // if the command is not active after execution, it is safe to send immediately.
            if (!myCommand->isActive()) {
                std::string message = myCommand->parseOut(); // CommandFilter::commandFilter(c);
                if (message.size() > 0) {
                    network->send(message.c_str());
                }
            }
        }
    }

    app.ExecuteActiveCommands();

    // Capture input from the nuklear GUI
    nk_input_begin(ctx);
    while (gui_window->pollEvent(event)) {
        // Our close event.
        // Note: We only have a 'minimize' button
        //       in our window right now, so this event is not
        //       going to fire.
        if (event.type == sf::Event::Closed) {
            nk_sfml_shutdown();
            gui_window->close();
            exit(EXIT_SUCCESS);
        }

            // Capture any keys that are released
        else if (event.type == sf::Event::KeyReleased) {
            // Check if the escape key is pressed.
            if (event.key.code == sf::Keyboard::Escape) {
                nk_sfml_shutdown();
                gui_window->close();
                exit(EXIT_SUCCESS);
            }
        }
        nk_sfml_handle_event(&event);
    }

    // Complete input from nuklear GUI
    nk_input_end(ctx);


    // Draw our GUI
    drawLayout(ctx, bg);

    // Where was the mouse previously before going to the next frame
    app.pmouseX = app.mouseX;
    app.pmouseY = app.mouseY;

    // retrieve and send completed commands
    Command *c2;
    do {
        c2 = app.getCompletedCommands();
        if (c2) {
            std::string message = c2->parseOut();
            if (message.size() > 0) {
                network->send(message);
            }
        }
    } while (c2);

    return gui_window->isOpen();
}


/*! \brief Draw the contents of the window.
 * <p> First draw all 'set' objects in the application (those in <code>App::userPictures</code>),
 *	   then draw all the objects in active commands, which may have partially complete pictures.
 *
 */
void draw(void) {
    // Static variable
    static int refreshRate = 0;
    static int refreshPerSecond = 0;
    ++refreshRate;    // Increment

    // We load into our texture the modified pixels
    // But we only do so every 10 draw calls to reduce latency of transfer
    // between the GPU and CPU.
    // Ask yourself: Could we do better with sf::Clock and refresh once
    // 	 	 every 'x' frames?
    if (refreshRate > 10) {
        app.GetTexture().loadFromImage(app.GetImage());
        refreshRate = 0;
    }

    // OpenGL is the background rendering engine,
    // so we are going to clear and draw our GUI graphics system.
    gui_window->setActive(true);
    gui_window->clear();
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    glClear(GL_COLOR_BUFFER_BIT);
    nk_sfml_render(NK_ANTI_ALIASING_ON);
    gui_window->display();

    // Draw to the canvas
    app.GetWindow().draw(app.GetSprite());

    // draw the user pictures from executed commands.
    for (sf::VertexArray *pic : app.getUserPictures()) {
        app.GetWindow().draw(*pic);
    }
    // draw the user pictures from active commands.
    for (Command *c : app.getActiveCommands()) {
        const std::vector<sf::VertexArray *> *command_pics = c->getDrawable();
        if (command_pics) {
            for (sf::VertexArray *pic : *command_pics) {
                app.GetWindow().draw(*pic);
            }
        }
    }
}

/*! \brief 	The entry point into our program.
*		
*/
int main(){
#ifdef _DEBUG
    std::cout << "DEBUG MESSAGES ENABLED.\n";
#endif

    // setup this runtime as either a client or server, according to user preference
    if (UDPTool::askUserIfServer()) {
#ifdef _DEBUG
        // this runtime is a server; execution will move to serverGo until program termination
        std::cout << "Hardcoded to use port 2000\n";

        network = UDPTool::getServer("2000");
#endif
#ifndef _DEBUG
        network = UDPTool::getServer();
#endif

        // run the server until program is terminated
        network->serverGo();
        return 0;
    }

    std::string clientName;
    // this runtime is a client; setup network client, then move to normal app execution

#ifdef _DEBUG
    // shorthand excution for local debugging purposes
    std::string clientPort;
    std::cout << "Enter client port: " << std::endl;
    std::getline(std::cin, clientPort);
    std::cout << "Enter client name for chat: " << std::endl;
    std::getline(std::cin, clientName);
    std::cout << "Hardcoded to use server: 127.0.0.1:2000, self: " << clientPort << "\n";
    network = UDPTool::getClient("127.0.0.1", "2000", clientPort);
#endif
#ifndef _DEBUG
    // normal execution, which asks for both local port and target server ip/port
    std::cout << "Enter client name for chat: " << std::endl;
    std::getline(std::cin, clientName);
    network = UDPTool::getClient();
#endif

    network->send("Join attempt."); // ping the server with a first time message

    // init the app
    app = App();

    app.connectingClientName = clientName;

    // Pass main's functions to the app
    app.Init(&initialization);
    app.UpdateCallback(&update);
    app.DrawCallback(&draw);

    // Call the main loop function
    app.Loop();

    // Cleanup
    app.Destroy();
    nk_sfml_shutdown(); // Terminate the Nuklear GUI library

    return 0;
}      
