#version 120  // Use an older GLSL version for compatibility

// Received from the vertex shader
varying vec4 vertexColor;  
varying vec3 vertexNormal;
varying vec3 vertexToLight[gl_MaxLights];


void main() {
    
    vec4 finalColor = vec4(0, 0, 0, 1);

    // Loop through all light sources
    for (int i = 0; i < gl_MaxLights; ++i) {
        // Ambient component
        vec4 ambient = gl_LightSource[i].ambient * vertexColor;

        // Diffuse component
        float diff = max(dot(normalize(vertexNormal), normalize(vertexToLight[i])), 0.0);
        vec4 diffuse = diff * gl_LightSource[i].diffuse * vertexColor;

        // Specular component
        // vec3 viewDir = normalize(-fragVertexPos);  // View direction in eye space
        // vec3 reflectDir = reflect(-lightDir, normal);
        // float spec = pow(max(dot(viewDir, reflectDir), 0.0), gl_FrontMaterial.shininess);
        // vec4 specular = spec * gl_LightSource[i].specular * gl_FrontMaterial.specular;

        // Accumulate lighting contributions
        finalColor += ambient + diffuse;
    }

    gl_FragColor = finalColor;  // Set the output color
    // gl_FragColor.r = length(vertexToLight[0]) / 10;  // Set the output color
    // gl_FragColor.g = 0; 
    // gl_FragColor.b = 0; 

    
    // gl_FragColor.rgb = normalize(vertexToLight[0]); 
    // gl_FragColor.a = 1; 
    // gl_FragColor.rgb = vec3(-gl_LightSource[0].position); // proof position is in view space
}