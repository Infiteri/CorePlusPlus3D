#version 330 core

uniform vec4 baseColorTop;    // Color for the top of the sky
out vec4 FragColor; // Output color

void main() {
    FragColor = vec4(baseColorTop); // Set the final color to be used in rendering
}
