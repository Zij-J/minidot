#include "error_handler.h"
#include <iostream> // for `cerr`

#define RED  "\x1B[31m" // color of error message
#define YEL  "\x1B[33m" 
#define GRE  "\x1B[32m" 
#define RESET "\x1B[0m"


void ErrorHandler::fatal_error(string message, string file_name, int executed_line) {
    cerr << RED "FATAL ERROR " RESET "in " << file_name << "(line " << executed_line << "): " << message << endl;
    exit(EXIT_FAILURE);
}


void ErrorHandler::warning(string message, string file_name, int executed_line) {
    cerr << YEL "WARNING " RESET "in " << file_name << "(line " << executed_line << "): " << message << endl;
}


void ErrorHandler::success_notice(string message, string file_name, int executed_line) {
    cout << GRE "OPERATION SUCCESS " RESET "in " << file_name << "(line " << executed_line << "): " << message << endl;
}