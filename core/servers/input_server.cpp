#include "input_server.h"

#include <typeinfo> // `typeid` keyword for C# `is` like behavior

InputServer *InputServer::singleton = nullptr;
bool InputServer::is_deleting = false; // singleton deletion by destructor will call twice, must avoid delete `nullptr` 