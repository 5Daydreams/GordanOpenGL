#include "Renderer.h"

void Renderer::Draw(const VAO& VArrayObject, const EBO& IndexBufferObject, const Shader& shader) const
{
    shader.Bind();
    VArrayObject.Bind();
    IndexBufferObject.Bind();

    glDrawElements(GL_TRIANGLES, IndexBufferObject.count, GL_UNSIGNED_INT, nullptr);   // Square
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}