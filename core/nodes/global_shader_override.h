// In Godot, shaders is resources instanced in material, which only affect the object it attaches. 
//      https://docs.godotengine.org/en/stable/tutorials/shaders/your_first_shader/your_first_2d_shader.html
// We simplify shaders by seperating them from mesh system, i.e., a  global shader  
// Godot has ShaderGlobalsOverride node. We follow the spirit and make a GlobalShaderOverrideOverride node for changing glew `program` parameter
//      https://docs.godotengine.org/en/stable/classes/class_shaderglobalsoverride.html
// ONLY override 3D shaders 

#ifndef GLOBAL_SHADER_OVERRIDE
#define GLOBAL_SHADER_OVERRIDE

#include <GL/glew.h>
#include <string>
#include "node.h"

using namespace std;


class GlobalShaderOverride: public Node
{
public:
    // file: path relative to exectuable
    GlobalShaderOverride(string vertex_file, string fragment_file);
    ~GlobalShaderOverride();

private:
    void on_tree_entered();
    // file: path relative to exectuable, shader_type: pass in "glCreateShader"
    void compile_and_link(string file, GLuint shader_type);
    
    GLuint program;
};



#endif