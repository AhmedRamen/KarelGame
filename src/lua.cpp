#include "lua/lua.hpp"
#include "sol/sol.hpp"

#pragma region Lua Script Implementation
int HandleException(lua_State* L, sol::optional<const std::exception&> exception, sol::string_view description) {
    //There may be an error, so let's print it to the console by any chance.
    std::cout << "An error occured.";
    if (exception) {
        std::cout << "Lua error: ";
        const std::exception& ex = *exception;
        std::cout << ex.what() << std::endl;
    }
    else {
        std::cout << "In-depth about what happened: ";
        std::cout.write(description.data(), static_cast<std::streamsize>(description.size()));
        std::cout << std::endl;
    }

    return sol::stack::push(L, description);
}


void StartMaus() {
    started = true;
    //Initalise Lua and its functions
    sol::state lua;
    std::cout << "Initalising Lua functions..." << std::endl;
    lua.open_libraries(sol::lib::base);
    lua.set_function("MoveForward", MoveForward);
    lua.set_function("TurnRight", TurnRight);
    lua.set_function("TurnLeft", TurnLeft);
    lua.set_function("TurnAround", TurnAround);
    std::cout << "Attempting to execute lua script..." << std::endl;
    //Try to execute the lua file
    sol::protected_function_result pfr = lua.safe_script_file("workers/MausWorker.lua", &sol::script_pass_on_error);

    //File successfully executed
    if (pfr.valid()) {
        std::cout << "Lua script successfully executed!" << std::endl;
        return;
    }
    //Print error if it does infact throw an exception
    sol_c_assert(!pfr.valid());
    sol::error err = pfr;
    std::cout << err.what() << std::endl;
    failed = true;
}


#pragma endregion