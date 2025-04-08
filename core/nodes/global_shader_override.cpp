#include "global_shader_override.h"

#include <fstream> // ifstream
#include <sstream> // stringstream
#include "../error/error_handler.h"
#include "../debugger/code_tester.h"
#include "../servers/render_server.h"

GlobalShaderOverride::GlobalShaderOverride(string vertex_file, string fragment_file):
program(glCreateProgram()) {
    tree_entered.add_listener<GlobalShaderOverride, on_tree_entered>(this);

    compile_and_link(vertex_file, GL_VERTEX_SHADER);
    compile_and_link(fragment_file, GL_FRAGMENT_SHADER);
}

GlobalShaderOverride::~GlobalShaderOverride() {
    RenderServer::get_singleton().set_shader_program_3d(0); // revert back to default
    glDeleteProgram(program);
}


void GlobalShaderOverride::on_tree_entered() {
    DEBUG_COUT(program);
    RenderServer::get_singleton().set_shader_program_3d(program);
}

void GlobalShaderOverride::compile_and_link(string file, GLuint shader_type) {

    // create shader
    GLuint shader = glCreateShader(shader_type);

    // read source file
    std::ifstream shader_file(file);
    ErrorHandler_FATAL_IF(!shader_file.is_open(), __FILE__, __LINE__);
    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf(); // turn internal ifstream buffer into readable stringstream
    string raw_shader_file = shader_stream.str(); // using `shader_stream.str().c_str()` causing `shader_stream.str()` freed when line end, making c_str invalid, must prevent!

    // open shader source
    const char *input_shader_stream = raw_shader_file.c_str(); // I have no idea why can't just pass c_str, but here we are, need to pass const * of c_str(which is const *char)
    glShaderSource(shader, 1, &input_shader_stream, nullptr); // 1: soure use one full string to represent, nullptr: use all string to compile

    GLint success; 
    // compile shader
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // check compilation errors
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        string error_message(" shader compilation failed\n");
        ErrorHandler::fatal_error(file + error_message + infoLog, __FILE__, __LINE__);
    }
    // link shader
    glAttachShader(program, shader);
    glGetShaderiv(shader, GL_LINK_STATUS, &success); // check link errors
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        string error_message(" shader linkage failed\n");
        ErrorHandler::fatal_error(file + error_message + infoLog, __FILE__, __LINE__);
    }
    glLinkProgram(program);

    // Clean ups
    glDeleteShader(shader); // after successful link, shader itself can be removed
    shader_file.close();
}