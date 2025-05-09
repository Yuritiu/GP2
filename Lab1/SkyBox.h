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

    void init(const std::vector<std::string>& faces);

    void draw(const Camera& camera);
    void drawReflect(const glm::mat4& view, const glm::mat4& projection);

    GLuint getCubeMapTexture() const { return cubemapTexture; }

private:
    GLuint loadCubemap(const std::vector<std::string>& faces);
    void setupMesh();

    GLuint _vao = 0;
    GLuint _vbo = 0;
    GLuint cubemapTexture = 0;
    Shader shader;
};