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
in vec2 v_texCoord; // Import our texture coordinates from vertex shader
in vec3 FragPos; // Import the fragment position
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// ======================= Globals ====================
// We will have another constant for specular strength
float specularStrength = 0.5;
const float diff_levels = 20.0;
const float spec_levels = 5.0;


void main()
{

  // The below code is derived heavily from: https://learnopengl.com/Advanced-Lighting/Normal-Mapping 
	// Store the texture coordinates
	vec3 normal = texture(u_DiffuseMap, v_texCoord).rgb;
	normal = normalize(normal);

  // Set the n_color (normal color) based on the diffuse map
	vec3 n_color = texture(u_DiffuseMap, v_texCoord).rgb;

  // Now we calculate the ambient, diffuse, and specular values
	// First, the ambient.
	// Ambient is simply color times a factor that adjusts how much the light is faded.
	float ambientFactor = 0.1;
	vec3 ambient = ambientFactor * n_color;


  // Second, the diffuse.
	// This is dependant on the direction of the light source and the surface normal
	vec3 lightDirection = normalize(TangentLightPos - TangentFragPos);

  float dotDif = dot(lightDirection, normal);
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
	vec3 diffuse = max(dotDif, 0.0) * n_color;

  // Third, the specular.
	// This depends on the viewing angle, the light source, and the reflectivity of the surface.
	float specularFactor = 0.005;
	float shininess = 16.0;
	vec3 viewDirection = normalize(TangentViewPos - TangentFragPos);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);

  dotDif = dot(normal, halfwayDirection);
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

	float spec = pow(max(dotDif, 0.0), shininess);
	vec3 specualar = vec3(specularFactor) * spec;

  // Add our three values together for the total lighting effect
	vec3 totalLighting = ambient + diffuse + specualar;

	FragColor = vec4(totalLighting, 1.0);
}
// ==================================================================
