/** 
 *  @file   test3.cpp 
 *  @brief  Unit test for checking that the update and draw function are called as exepected in the loop.
 *  @author Mike and John
 *  @date   2020-12-03
 * referenced https://medium.com/@junaidrahim/a-guide-to-using-catch2-for-unit-testing-in-c-f0f5450d05fb
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


namespace test3 {
	App app;
	const int END_LOOP = 10;

	/*
	state variable:
	0 is default state
	1 is initialization state
	2 is update state
	3 is draw state
	*/
	int state = 0;
	int loop_count = 0;

	/*! \brief 	Modified init call
	*
	*/
    void initialization(void) {
        std::cout << "Starting the App" << std::endl;
		// init should be called first.
		static bool init_called_once = false;
		if (!init_called_once) {
			//GIVEN("init_called_once = " + std::to_string(init_called_once) + "; Expected last state to have been default state (0).") 
			{
				REQUIRE(state == 0);
			}
			init_called_once = true;
			//inc state
			state = 1;
		}
		else {
			//GIVEN("init_called_once = " + std::to_string(init_called_once) + "; The init function was called twice.") 
			{
				REQUIRE(false);
			}
		}
    }

	/*! \brief 	Modified update call
	*
	*/
	bool update(void) {
		loop_count++;
		static bool init_found_once = false;
		// SECTION("Entered Update, init_found_once = " + std::to_string(init_found_once)) {
			std::cout << loop_count << std::endl;
			// Update our canvas; here, check and increment the state.

			if (!init_found_once) {
				init_found_once = true;
				// init should be called first.
				//GIVEN("Expected last state to have been init state.") 
				{
					REQUIRE(state == 1);
				}
				state = 2;
			}
			else {
				// else, can only be reach via draw function
				//>>GIVEN("Expected last state to have been draw state.") 
				{
					REQUIRE(state == 3);
				}
				state = 2;
			}
			if (loop_count == END_LOOP) {
				// closing the window should exit the program normally
				app.GetWindow().close();
			}
			else if (loop_count > END_LOOP) {
				// to prevent an endless test case, abort on bizarre behavior
				//GIVEN("Did not exit loop on window close.") 
				{
					REQUIRE(false);
				}
				//throw "Did not exit loop on window close.";  // --------------- failing to throw fails to exit test

			}
		// }
		return true;
	}

	int refreshRate = 0;
	/*! \brief 	Modified draw call
	*
	*/
	void draw(void) {
		// Update our canvas; here, check and increment the state.
		// ensure that this works for 10 loop iterations.

		// init should be called first.
		GIVEN("Expected last state to have been update state.") 
		{
			REQUIRE(state == 2);
		}
		state = 3;
	}
 
	/*! \brief 	Initialize and destroy the program
	*
	*/
	TEST_CASE("draw and update called as expected"){
		app.Init(&initialization);
		app.UpdateCallback(&update);
		app.DrawCallback(&draw);
		app.Loop();
		//GIVEN("Expected to be in draw state when exiting loop") 
		{
			REQUIRE(state == 3);
		}
		// Destroy our app
		app.Destroy();
	}
}