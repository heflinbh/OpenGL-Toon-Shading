#include "Object.hpp"
#include "Camera.hpp"
#include "Error.hpp"


Object::Object(std::string fileName){
        // Open the file of interest
	std::ifstream inFile;
	inFile.open(fileName);

	// Read and parse the file
	if(inFile.is_open()) {

                std::cout << "Opening " << fileName << std::endl;
		
		// For every line, we will determine if it corresponds to a vertex, normal, or face.
		// After this determination, the data will be collected.
                // Below code was derived from: https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
		std::string line;
		while(getline(inFile, line)) {

                        if(line.find("mtllib") != std::string::npos) {
                                std::string mtlName = line.substr(7);
                                int rootPathIndex = fileName.find_last_of('/');
                                mtlPath = fileName.substr(0,rootPathIndex+1) + mtlName;    
                        }

                        if (line.substr(0,2) == "v ") {

                                // std::cout << line << std::endl;
                                std::istringstream v(line.substr(2));
                                float x,y,z;
                                v>>x;v>>y;v>>z;

                                vertices.push_back(glm::vec3(x,y,z));
                        }

                        else if (line.substr(0,2) == "vt") {

                                std::istringstream v(line.substr(3));
                                float s,t;
                                v>>s;v>>t;
                                textures.push_back(glm::vec2(s,t));
                        }

                        else if (line.substr(0,2) == "vn") {

                                // std::cout << line << std::endl;
                                std::istringstream v(line.substr(2));
                                float x,y,z;
                                v>>x;v>>y;v>>z;

                                normals.push_back(glm::vec3(x,y,z));
                        }

                        else if (line.substr(0,2) == "f ") {

                                // std::cout << line << std::endl;
                                std::istringstream v(line.substr(2));
                                GLuint a,b,c;

                                std::string aa[3];
                                v>>aa[0];v>>aa[1];v>>aa[2];

                                for(int i=0;i<3;i++){
                                        std::istringstream v(aa[i]);
                                        v>>a;
                                        if (v.peek() == '/')
                                        v.ignore();
                                        v>>b;
                                        if (v.peek() == '/')
                                        v.ignore();
                                        v>>c;
                                        a--;b--;c--;

                                        m_indices.push_back(m_indices.size());
                                        glm::vec3 vect = vertices[a];
                                        glm::vec2 text = textures[b];
                                        glm::vec3 norm = normals[c];

                                        for(int j=0; j < 3; j++) {
                                                m_vertices.push_back(vect[j]);
                                        }  
                                        for(int k=0; k < 2; k++) {
                                                m_vertices.push_back(text[k]);
                                        }
                                        for(int l=0; l < 3; l++) {
                                                m_vertices.push_back(norm[l]);
                                        }
                                }
                        }    
                }
        
                ParseMTL();
                inFile.close();
        }
}

Object::~Object(){
    
}

// TODO: In the future it may be good to 
// think about loading a 'default' texture
// if the user forgets to do this action!
void Object::LoadTexture(std::string fileName){
        // Load our actual textures
        m_textureDiffuse.LoadTexture(fileName);
}

// Initialization of object as a 'quad'
//
// This could be called in the constructor or
// otherwise 'explicitly' called this
// so we create our objects at the correct time
void Object::MakeTexture(){

        for(int i=0; i < m_vertices.size(); i+=8) {
                
                m_geometry.AddVertex(m_vertices[i],
                                     m_vertices[i+1],
                                     m_vertices[i+2],
                                     m_vertices[i+3],
                                     m_vertices[i+4],
                                     m_vertices[i+5],
                                     m_vertices[i+6],
                                     m_vertices[i+7]);
        }

        for(int j=0; j < m_indices.size(); j+=3) {
                m_geometry.MakeTriangle(m_indices[j], m_indices[j+1], m_indices[j+2]);
        }

        

        // This is a helper function to generate all of the geometry
        m_geometry.Gen();

        // Create a buffer and set the stride of information
        // NOTE: How we are leveraging our data structure in order to very cleanly
        //       get information into and out of our data structure.
        m_vertexBufferLayout.CreateNormalBufferLayout(m_geometry.GetBufferDataSize(),
                                        m_geometry.GetIndicesSize(),
                                        m_geometry.GetBufferDataPtr(),
                                        m_geometry.GetIndicesDataPtr());

        // Load our actual texture
        // We are using the input parameter as our texture to load
        m_textureDiffuse.LoadTexture(ppmPath.c_str());
}

// Bind everything we need in our object
// Generally this is called in update() and render()
// before we do any actual work with our object
void Object::Bind(){
        // Make sure we are updating the correct 'buffers'
        m_vertexBufferLayout.Bind();
        // Diffuse map is 0 by default, but it is good to set it explicitly
        m_textureDiffuse.Bind(0);
}

// Render our geometry
void Object::Render(){
    // Call our helper function to just bind everything
    Bind();
	//Render data
    glDrawElements(GL_TRIANGLES,
                   m_geometry.GetIndicesSize(), // The number of indicies, not triangles.
                   GL_UNSIGNED_INT,             // Make sure the data type matches
                        nullptr);               // Offset pointer to the data. 
                                                // nullptr because we are currently bound
}

void Object::ParseMTL() {
        std::ifstream inFile;
        //inFile.open(mtlPath);

        if(mtlPath.back() == ' ' ||
           mtlPath.back() == '\\' || 
           mtlPath.back() == '\r' || 
           mtlPath.back() == '\n' ||
           mtlPath.back() == '\t' ||
           mtlPath.back() == '\v' ||
           mtlPath.back() == '\f') {
                mtlPath = mtlPath.substr(0, mtlPath.size()-1);
        }

        inFile.open(mtlPath);

        if(inFile.is_open()) {
                std::cout << "Opening " << mtlPath << std::endl;
                std::string line;
                while(getline(inFile, line)) {
                        if(line.find("map_Kd") != std::string::npos) {
                                std::string ppmName = line.substr(7);
                                std::cout << "PPM Name: " << ppmName << std::endl;

                                int rootPathIndex = mtlPath.find_last_of('/');

                                ppmPath = mtlPath.substr(0, rootPathIndex+1) + ppmName;
                                std::cout << "PPM Path: " << ppmPath << std::endl;
                        }
                }
        }
        inFile.close();
}

