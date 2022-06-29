// ==================================================================
#version 330 core
// Read in our attributes stored from our vertex buffer object
// We explicitly state which is the vertex information
// (The first 3 floats are positional data, we are putting in our vector)
layout(location=0)in vec3 position; 
layout(location=1)in vec3 normals; // Our second attribute - normals.
layout(location=2)in vec2 texCoord; // Our third attribute - texture coordinates.
layout(location=3)in vec3 tangents; // Our third attribute - texture coordinates.
layout(location=4)in vec3 bitangents; // Our third attribute - texture coordinates.

// If we are applying our camera, then we need to add some uniforms.
// Note that the syntax nicely matches glm's mat4!
uniform mat4 model; // Object space
uniform mat4 view; // Object space
uniform mat4 projection; // Object space
uniform vec3 lightPos;

// Export our normal data, and read it into our frag shader
out vec3 myNormal;
// Export our Fragment Position computed in world space
out vec3 FragPos;
// If we have texture coordinates we can now use this as well
out vec2 v_texCoord;

out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;


void main()
{


    // The below code is derived heavily from: https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	FragPos = vec3(model * vec4(position, 1.0));
	v_texCoord = texCoord;

    // Below we calculate the TBN vector (Tangent, Bitangent, Normal)
	mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * tangents);
    vec3 N = normalize(normalMatrix * normals);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    // With our TBN matrix, we can calculate the tangents of our light, view and fragment positions.
	// These values are then sent out to the fragment shader to use in the calculations for ambient, diffuse, and specular light effects
	mat3 TBN = transpose(mat3(T, B, N));
	TangentLightPos = TBN * lightPos;
	TangentViewPos = TBN * lightPos;
	TangentFragPos = TBN * FragPos;

    gl_Position = projection * view * model * vec4(position, 1.0f);
}
// ==================================================================
