// ====================================================
#version 330 core

// ======================= uniform ====================
// Our light sources
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientIntensity;
// Used for our specular highlights
uniform mat4 view;
// If we have texture coordinates, they are stored in this sampler.
uniform sampler2D u_DiffuseMap; 

// ======================= IN =========================
in vec3 myNormal; // Import our normal data
in vec2 v_texCoord; // Import our texture coordinates from vertex shader
in vec3 FragPos; // Import the fragment position

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// ======================= Globals ====================
// We will have another constant for specular strength
float specularStrength = 0.5;
const float diff_levels = 5.0;
const float spec_levels = 1000.0;


void main()
{
    // Store our final texture color
    vec3 diffuseColor;
    diffuseColor = texture(u_DiffuseMap, v_texCoord).rgb;

    // (1) Compute ambient light
    vec3 ambient = ambientIntensity * lightColor;

    // (2) Compute diffuse light

    // Compute the normal direction
    vec3 norm = normalize(myNormal);
    // From our lights position and the fragment, we can get
    // a vector indicating direction
    // Note it is always good to 'normalize' values.
    vec3 lightDir = normalize(lightPos - FragPos);
    // Now we can compute the diffuse light impact
    float dotDif = dot(norm, lightDir);

    float upper_bound = 1.0;
    for(int i=1; i <= diff_levels; i++) {
		float lower_bound = 1.0 * ((diff_levels - i) / diff_levels);

		if(dotDif <= upper_bound && dotDif > lower_bound) {
			dotDif = lower_bound;
			break;
		}
		else {
			upper_bound = lower_bound;
		}
	}

    float diffImpact = max(dotDif, 0.0);
    vec3 diffuseLight = diffImpact * lightColor;
    

    // (3) Compute Specular lighting
    vec3 viewPos = vec3(0.0,0.0,0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    dotDif = dot(viewDir, reflectDir);
    upper_bound = 1.0;
    for(int i=1; i <= spec_levels; i++) {
		float lower_bound = 1.0 * ((spec_levels - i) / spec_levels);

		if(dotDif <= upper_bound && dotDif > lower_bound) {
			dotDif = upper_bound;
			break;
		}
		else {
			upper_bound = lower_bound;
		}
	}

    float spec = pow(max(dotDif, 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Our final color is now based on the texture.
    // That is set by the diffuseColor
    vec3 Lighting = diffuseLight + ambient + specular;

    FragColor = vec4(diffuseColor * Lighting,1.0);
}
// ==================================================================
