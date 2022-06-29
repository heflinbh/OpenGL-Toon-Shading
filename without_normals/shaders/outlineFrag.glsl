// ====================================================
#version 330 core

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

void main()
{
    // Final color + "how dark or light to make fragment"
    FragColor = vec4(0.0, 0.0, 0.0 ,1.0);
}
// ==================================================================
