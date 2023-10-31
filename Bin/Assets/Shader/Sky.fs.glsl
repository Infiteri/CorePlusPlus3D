#version 330 core

uniform vec4 color;    // Base color
uniform vec2 Time;         // Time information (assuming y-component for top/bottom darkening)

out vec4 FragColor;        // Output color

void main() {
    // Simulating a darkening effect based on the y-component of Time
    float gradient = abs(sin(Time.y)); // Use sine of y-coordinate to create a gradient

    // Darken the color towards the top and bottom
    vec3 modifiedColor = vec3(color) * (1.0 - 0.5 * gradient);

    FragColor = vec4(modifiedColor, 1.0); // Set the final color to be used in rendering
}
