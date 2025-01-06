#version 120  // Use an older GLSL version for compatibility

// Received from the vertex shader
varying vec4 vertexColor;  
varying vec3 fragNormal;
varying vec3 lightToFrag[gl_MaxLights];


void main() {
    
    vec4 finalColor = vec4(0, 0, 0, 1);

    // Loop through all light sources
    for (int i = 0; i < gl_MaxLights; ++i) {
        vec3 lightPos = vec3(gl_LightSource[i].position);  // Light position in eye space
        // vec3 lightDir = normalize(lightPos - fragVertexPos);

        // Ambient component
        vec4 ambient = gl_LightSource[i].ambient * vertexColor;

        // Diffuse component
        float diff = max(dot(normalize(fragNormal), normalize(lightToFrag[i])), 0.0);
        // float diff = 0.0;
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

}