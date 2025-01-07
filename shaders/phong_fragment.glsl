#version 120  // Use an older GLSL version for compatibility

// Received from the vertex shader
varying vec4 vertexColor;  
varying vec4 vertexPos;
varying vec3 vertexNormal;
varying vec3 vertexToLight[gl_MaxLights];


void main() {
    
    vec4 finalColor = vec4(0, 0, 0, 1);

    // Loop through all light sources
    for (int i = 0; i < gl_MaxLights; ++i) {
        // Ambient component
        vec4 ambient = gl_LightSource[i].ambient;

        // Diffuse component
        float diff = max(dot(normalize(vertexNormal), normalize(vertexToLight[i])), 0.0);
        float threshold_diff = smoothstep(0.245, 0.255, diff);      // cel shading! by threshold 25.5 & smooth interpolation between 0.245 to 0.255 
        vec4 diffuse = threshold_diff * gl_LightSource[i].diffuse;

        // Specular component
        vec3 vertexToEye = normalize(-vec3(vertexPos));  // View direction in view space
        vec3 reflectLight = reflect(-normalize(vertexToLight[i]), normalize(vertexNormal)); // reflacted light in view space
        float spec = pow(max(dot(vertexToEye, reflectLight), 0.0), 5); // fixed, power 5 time
        float threshold_spec = smoothstep(0.621, 0.629, spec);      // cel shading again! 
        vec4 specular = threshold_spec * gl_LightSource[i].specular;

        // Accumulate lighting contributions
        finalColor += (ambient + diffuse + specular) * vertexColor;
    }

    gl_FragColor = finalColor;  // Set the output color
    // gl_FragColor.r = length(vertexToLight[0]) / 10;  // Set the output color
    // gl_FragColor.g = 0; 
    // gl_FragColor.b = 0; 

    
    // gl_FragColor.rgb = normalize(vertexToLight[0]); 
    // gl_FragColor.a = 1; 
    // gl_FragColor.rgb = vec3(-gl_LightSource[0].position); // proof position is in view space
}