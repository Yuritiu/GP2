#include "SkyBox.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/gtc/type_ptr.hpp>


SkyBox::~SkyBox() 
{
    if (_vao) glDeleteVertexArrays(1, &_vao);
    if (_vbo) glDeleteBuffers(1, &_vbo);
    if (cubemapTexture) glDeleteTextures(1, &cubemapTexture);
}

void SkyBox::init(const std::vector<std::string>& faces) 
{
    // Load the cubemap texture
    cubemapTexture = loadCubemap(faces);

    // Load and compile skybox shader
    shader.init("..\\res\\skybox.vert", "..\\res\\skybox.frag");

    shader.Bind();
    GLint loc = glGetUniformLocation(shader.getID(), "skybox");
    glUniform1i(loc, 0);  

    // Prepare cube geometry
    setupMesh();
}

void SkyBox::draw(const Camera& camera) 
{
    glDepthFunc(GL_LEQUAL);       
    glDepthMask(GL_FALSE);        

    glDisable(GL_CULL_FACE);

    shader.Bind();
    glm::mat4 view = glm::mat4(glm::mat3(camera.getView())); 
    glm::mat4 proj = camera.getProjection();

    GLint loc = glGetUniformLocation(shader.getID(), "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
    loc = glGetUniformLocation(shader.getID(), "projection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(proj));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
}

GLuint SkyBox::loadCubemap(const std::vector<std::string>& faces) 
{
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    int width, height, nrChannels;
    for (GLuint i = 0; i < faces.size(); i++) {
        int w, h, n;
        unsigned char* data = stbi_load(faces[i].c_str(), &w, &h, &n, 0);
        
        /*std::cout << "Loading face[" << i << "] = " << faces[i]
            << " (" << w << "x" << h << ", " << n << " channels) → "
                << (data ? "OK" : "FAILED") << "\n";*/

        if (data) {
            // choose the right format
            GLenum fmt = (n == 4 ? GL_RGBA : GL_RGB);
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,             // level
                fmt,           // internal format
                w, h,
                0,             // border
                fmt,           // source format
                GL_UNSIGNED_BYTE,
                data
            );
            stbi_image_free(data);
        }
        else {
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texID;
}

void SkyBox::setupMesh() 
{
    float skyboxVertices[] = 
    {
        // positions for a cube
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

