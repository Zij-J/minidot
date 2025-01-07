#version 120

// only support 1 light source

// Notes:
// 1. CRLF, non-ASCII  mess up the compiler, change to LF and ASCII
// 2. #version must be the first token

// varibales be interpolated to frag
varying vec4 vertexColor;
// varying vec4 vertexPos;
varying vec3 vertexNormal;
varying vec3 vertexToLight[gl_MaxLights]; // every light source need one


void main() {
    // same as default (Viewspace + projection)
    vec4 veiw_vertex = gl_ModelViewMatrix * gl_Vertex;
    gl_Position = gl_ProjectionMatrix * veiw_vertex;

    // just pass colors (will interpolated too)
    vertexColor = gl_Color;

    // vectors (do normalize in fragemnt shader, interperated vector will correctly nomalize)
    vertexNormal = gl_NormalMatrix  * gl_Normal; // transform normal to view space normal(* scale & shear only)

    for (int i = 0; i < gl_MaxLights; ++i) {
        vertexToLight[i] = vec3(gl_LightSource[i].position) - vec3(veiw_vertex); // in view space too (gl_LightSource[i].position already in)
        // vertexToLight[i] = vec3((gl_LightSource[i].position - gl_Vertex));
    }
} 