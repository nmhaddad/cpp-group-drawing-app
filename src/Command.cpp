/** 
 *  @file   Command.cpp 
 *  @brief  Implementation of Command.hpp
 *  @author Mike and John
 *  @date   2020-02-09 
 ***********************************************/

#ifndef COMMAND_CPP
#define COMMAND_CPP

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
// Include standard library C++ libraries.
#include <cstring>
#include <string>
#include <iostream>
// Project header files
#include "Command.hpp"
#include "App.hpp" // resolve forward declaration


/*! \brief  Print "Execute: " + the command's description, then execute the command.
 *
 * @return True, if the command successfully executed.
 *
 */
bool Command::execute() {
    if (!isActive()) {
#ifdef _DEBUG
        std::cout << "Execute: " << this << " " + m_commandDescription << std::endl;
#endif
    }
    return silent_execute();
};

/*! \brief  Print "Undo: " + the command's description, then undo the command.
 *
 * @return True, if the command is successfully undone.
 *
 */
bool Command::undo() {
#ifdef _DEBUG
    std::cout << "Undoing: " << m_commandDescription << std::endl;
#endif
    return silent_undo();
};


/*! \brief  Terminate some potentially active command, pushing its changes to <code>App</code>.
 *
 */
void Command::terminate() {};


/*! \brief  Inform the command of some event.
 *
 *	@param[in] event Some event to inform the command of.
 *
 */
void Command::inform(sf::Event event) {};

/*! \brief Default destructor for a generic command.  
 * <p> No change from default destructor, since it is capable
 *		of memory managment for this program.
 *
 */
Command::~Command() {
};


/** \brief Adjust this command's fields to match the given input.
    e.g. parseIn = "0Generic Command Description\n000\0"
    e.g. parseIn = "0Generic Command Description\n000,50,50,255,255,255;25,25,255,255,255...."
 * @param [in] commandDescription The command as a parsed string.
 **/
Command *Command::parseIn(std::string input) {

    is_server_command = true;

    if (input == "") {
        return this;
    }



    // Turn character array into string
    std::string parseString = input;

    // Create vector of strings for easier iteration
    std::vector <std::string> parseVector;
    std::istringstream is(parseString);
    std::string text;
    while (is >> text) {
        parseVector.push_back(text);
    }

    // Pop parseInt value
    unsigned int commandType = std::stoi(parseVector.front());
    parseVector.erase(parseVector.begin());

    // Read in undoAble value
    if (parseVector.front() == "0") undoAble = false;
    if (parseVector.front() == "1") undoAble = true;
    parseVector.erase(parseVector.begin());

    // Read in redoAble value
    if (parseVector.front() == "0") redoAble = false;
    if (parseVector.front() == "1") redoAble = true;
    parseVector.erase(parseVector.begin());

    if (parseVector.size() != 0) {
        // Read in color
        sf::Color color = sf::Color::Magenta;
        std::string str_color = parseVector.front();
        parseVector.erase(parseVector.begin());

        switch (str_color.c_str()[0]) {
            case '0':
                color = sf::Color::Black;
                break;
            case '1':
                color = sf::Color::Black;
                break;
            case '2':
                color = sf::Color::White;
                break;
            case '3':
                color = sf::Color::Red;
                break;
            case '4':
                color = sf::Color::Green;
                break;
            case '5':
                color = sf::Color::Blue;
                break;
            case '6':
                color = sf::Color::Yellow;
                break;
            case '7':
                color = sf::Color::Magenta;
                break;
            case '8':
                color = sf::Color::Cyan;
                break;
            case '9': {
                std::string r, g, b, a;
                r = parseVector.front();
                parseVector.erase(parseVector.begin());
                g = parseVector.front();
                parseVector.erase(parseVector.begin());
                b = parseVector.front();
                parseVector.erase(parseVector.begin());
                a = parseVector.front();
                parseVector.erase(parseVector.begin());
                color = sf::Color(std::stoi(r), std::stoi(g), std::stoi(b), std::stoi(a));
                break;
            }
            default:
                throw "Whoopsee! Should have got a char here. We're not sure what happened";
        }

        // Read in vertices
        // DrawCommand
        if (!parseVector.empty() && (commandType == CommandEnum::DRAW || commandType == CommandEnum::RAINBOWDRAW)) {

            drawable = new std::vector<sf::VertexArray*>();

            // point array
            drawable->push_back(new sf::VertexArray());
            drawable->back()->append(sf::Vertex(sf::Vector2f(std::stoi(parseVector[0]), std::stoi(parseVector[1])), color));

            // line array
            drawable->push_back(new sf::VertexArray(sf::LineStrip, 0));
            parseVector.erase(parseVector.begin());
            parseVector.erase(parseVector.begin());


            if (!parseVector.empty()) {
                int pair = 0;
                int x, y;
                for (std::vector<std::string>::iterator it = parseVector.begin(); it != parseVector.end(); ++it) {
                    if (pair == 0) {
                        x = std::stoi(*it);
                        pair++;
                    } else if (pair == 1) {
                        y = std::stoi(*it);
                        drawable->back()->append(sf::Vertex(sf::Vector2f(x, y), color));
                        pair = 0;
                    }
                }
            }
        } else if (!parseVector.empty() && commandType == CommandEnum::FILL) {

            parseVector.erase(parseVector.begin());
            parseVector.erase(parseVector.begin());

            // setup drawable
            drawable = new std::vector<sf::VertexArray*>();

            sf::VertexArray* rect = new sf::VertexArray(sf::Quads, 4);
            drawable->push_back(rect);
            // create the rectangle
            rect->append(sf::Vertex(sf::Vector2f(sf::Vector2i(std::stoi(parseVector[0]), std::stoi(parseVector[1]))), color));
            rect->append(sf::Vertex(sf::Vector2f(sf::Vector2i(std::stoi(parseVector[2]), std::stoi(parseVector[3]))), color));
            rect->append(sf::Vertex(sf::Vector2f(sf::Vector2i(std::stoi(parseVector[4]), std::stoi(parseVector[5]))), color));
            rect->append(sf::Vertex(sf::Vector2f(sf::Vector2i(std::stoi(parseVector[6]), std::stoi(parseVector[7]))), color));
        } else {
            drawable = nullptr;
        }

    }

    return this;
};


/** \brief Convert contents to readable, parsable char*.
    e.g. parseIn = "0Generic Command Description\n000,50,50,255,255,255;25,25,255,255,255...."
    "<0 for generic command><command description>\n<undoable 0/1>..."
    if parseOut is called by a child, that child will handle it's own data, then append this parent call to it's butt.
    "<some child command, e.g. 1><that command's junk>...<0 for generic command><command description>\n<undoable 0/1>..."
 * @return the command as a parsed string
 * */
    const char *Command::parseOut() {
        /**
         * 1. parseInt 1,2,3,4,5,6
         * 2. undoAble 0 true, 1 false
         * 3. redoAble 0 true, 1 false
         * 4. drawable ints
         */

        // parent parsing
        std::string builder = "";

        if (!undoAble) builder += "0 ";
        if (undoAble) builder += "1 ";
        if (!redoAble) builder += "0 ";
        if (redoAble) builder += "1 ";

        if (drawable) {
            // drawable is two VertexArrays (made of Vertex (made of Vector2f and Color and potentially texture
            // do stuff
            sf::VertexArray *square_vertexArray;
            sf::VertexArray *line_vertexArray;
            square_vertexArray = drawable->front();

            // get point info
            sf::VertexArray square_array = square_vertexArray[0];
            sf::Vertex corner_vertex = square_array[0];
            sf::Vector2f point_vector = corner_vertex.position;

            // color info
            sf::Color point_color = corner_vertex.color;
            if (point_color == sf::Color::Black) builder += "1 ";
            else if (point_color == sf::Color::White) builder += "2 ";
            else if (point_color == sf::Color::Red) builder += "3 ";
            else if (point_color == sf::Color::Green) builder += "4 ";
            else if (point_color == sf::Color::Blue) builder += "5 ";
            else if (point_color == sf::Color::Yellow) builder += "6 ";
            else if (point_color == sf::Color::Magenta) builder += "7 ";
            else if (point_color == sf::Color::Cyan) builder += "8 ";
            else {
                builder += "9 " + std::to_string(point_color.r) + " "
                           + std::to_string(point_color.g) + " "
                           + std::to_string(point_color.b) + " "
                           + std::to_string(point_color.a) + " ";
            }

            builder += std::to_string(point_vector.x) + " ";
            builder += std::to_string(point_vector.y) + " ";

            // get line info
            line_vertexArray = drawable->back();
            sf::VertexArray line_array = *line_vertexArray;
            for (int i = 0; i < line_array.getVertexCount(); i++) {
                sf::Vertex line_vertex = line_array[i];
                sf::Vector2f line_vector = line_vertex.position;
                sf::Color line_color = line_vertex.color;
                builder += std::to_string(line_vector.x) + " ";
                builder += std::to_string(line_vector.y) + " ";
            }
        }

        char *cstr = new char[builder.length() + 1];
        std::strcpy(cstr, builder.c_str());

        return cstr;
    };

#endif

