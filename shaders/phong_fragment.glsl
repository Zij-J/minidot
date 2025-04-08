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
        // attenuation factor: (1/distance)^2
        float attenuation = pow(1.0 / length(vertexToLight[i]), 2);

        // Ambient component
        vec4 ambient = gl_LightSource[i].ambient;

        // Diffuse component
        float diff = max(dot(normalize(vertexNormal), normalize(vertexToLight[i])), 0.0) * attenuation;
        float threshold_diff = smoothstep(0.49, 0.51, diff);      // cel shading! by threshold 0.5 & smooth interpolation between 0.45 to 0.55 
        vec4 diffuse = threshold_diff * gl_LightSource[i].diffuse;

        // Specular component
        vec3 vertexToEye = normalize(-vec3(vertexPos));  // View direction in view space
        vec3 reflectLight = reflect(-normalize(vertexToLight[i]), normalize(vertexNormal)); // reflacted light in view space
        float spec = pow(max(dot(vertexToEye, reflectLight), 0.0), 15) * attenuation; // fixed, power 10 time
        float threshold_spec = smoothstep(0.621, 0.629, spec);      // cel shading again! 
        vec4 specular = threshold_spec * gl_LightSource[i].specular;

        // Rim
        float rim = (max(-dot(vertexToEye, normalize(vertexNormal)), -0.3) + 0.3) * attenuation * max(dot(normalize(vertexToLight[i]), normalize(vertexNormal)), 0); // +0.3: facing a bit still rim it.
        float rimStrength = 0.7;
        float rimThres = 0.1;   
        rim = smoothstep(rimThres - 0.01, rimThres + 0.01, rim) * rimStrength;
        
        // Accumulate lighting contributions
        finalColor += (ambient + diffuse + specular + rim) * vertexColor;
    }

    gl_FragColor = finalColor;  // Set the output color
    // gl_FragColor.r = length(vertexToLight[0]) / 10;  // Set the output color
    // gl_FragColor.g = 0; 
    // gl_FragColor.b = 0; 

    
    // gl_FragColor.rgb = normalize(vertexToLight[0]); 
    // gl_FragColor.a = 1; 
    // gl_FragColor.rgb = vec3(-gl_LightSource[0].position); // proof position is in view space
}