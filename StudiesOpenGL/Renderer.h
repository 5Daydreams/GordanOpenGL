#pragma once

#include <glad/glad.h>
#include "VAO.h"
#include "EBO.h"
#include "shaderClass.h"

class Renderer
{
public:
    void Draw(const VAO& va, const EBO& ib, const Shader& shader) const;
    void Clear() const;
};