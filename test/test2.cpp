/** 
 *  @file   test2.cpp 
 *  @brief  Unit test for checking that publically available accessors don't return null.
 *  @author Mike and John
 *  @date   2020-12-03
 ***********************************************/

#include "catch.hpp"

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Include standard library C++ libraries.
#include <iostream>
#include <string>
// Project header files
#include "App.hpp"
#include "Command.hpp"


namespace test2 {
    void initialization(void) {
        std::cout << "Starting the App" << std::endl;
    }

    /*! \brief 	Initialize and destroy the program
    *		
    */
    TEST_CASE("check accessors are assigned"){	
        App app;
        app.Init(&initialization);

        REQUIRE(&app.GetImage() != nullptr);
        REQUIRE(&app.GetTexture() != nullptr);
        REQUIRE(&app.GetWindow() != nullptr);
    
        app.Destroy();
    }
}