#version 120

// only support 1 light source

// Notes:
// 1. CRLF, non-ASCII  mess up the compiler, change to LF and ASCII
// 2. #version must be the first token

// varibales be interpolated to frag
varying vec4 vertexColor;
varying vec3 fragNormal;
varying vec3 lightToFrag[gl_MaxLights]; // every light source need one


void main() {
    // same as default
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // just pass colors (will interpolated too)
    vertexColor = gl_Color;

    // vectors (do normalize in fragemnt shader, interperated vector will correctly nomalize)
    fragNormal = gl_NormalMatrix * gl_Normal; // transform normal to global vector normal(* scale & shear only)

    for (int i = 0; i < gl_MaxLights; ++i) {
        lightToFrag[i] = vec3(gl_Position) - vec3(gl_LightSource[i].position);
    }
} 