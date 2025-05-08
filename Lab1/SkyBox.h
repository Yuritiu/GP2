#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"

class SkyBox 
{
public:
    SkyBox() = default;
    ~SkyBox();

    // Initialize with 6 cubemap faces (order: +X, -X, +Y, -Y, +Z, -Z)
    void init(const std::vector<std::string>& faces);

    // Draw the skybox using the provided camera
    void draw(const Camera& camera);

private:
    GLuint loadCubemap(const std::vector<std::string>& faces);
    void setupMesh();

    GLuint _vao = 0;
    GLuint _vbo = 0;
    GLuint cubemapTexture = 0;
    Shader shader;
};