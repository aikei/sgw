#include "OpenGLRenderer.hpp"
#include "FlushList.hpp"

#include <sgw/geom/AllShapes.hpp>
#include <utils/utils.hpp>
#include <math/math.hpp>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <stdexcept>

#include <GL/glew.h>

GLuint vertexBuffer = 0;
GLuint indexBuffer = 0;

GLuint primitiveRestartIndex = 65000;
GLuint gWorldLocation;

sgw::Mat4 perspectiveTransform;

sgw::OpenGLRenderer::OpenGLRenderer()
#ifdef SGW_VIRTUAL_RENDERER_INTERFACE
    : BaseRenderer()
#endif
{

}

static GLenum ConvertFlushListType(int type)
{
    switch(type)
    {
        case sgw::FlushList::TYPE_LINES:
            return GL_LINES;
            break;
            
        case sgw::FlushList::TYPE_LINE_LOOP:
            return GL_LINES;
            break;
            
        case sgw::FlushList::TYPE_LINE_STRIP:
            return GL_LINE_STRIP;
            break;
            
        case sgw::FlushList::TYPE_FILLED_TRIANGLES:
        case sgw::FlushList::TYPE_RECTANGLES:
        case sgw::FlushList::TYPE_TRIANGLES:        
            return GL_TRIANGLES;
            break;
    }
}

const char* vertexShader =
"#version 330\n"
"layout (location = 0) in vec3 position;"
"uniform mat4 gWorld = mat4(1.0);"
"void main()"
"{"
"    gl_Position = gWorld*vec4(position, 1.0);"
"}";

const char* fragmentShader =
"#version 330\n"
"out vec4 fragColor;"
"void main()"
"{"
"    fragColor = vec4(1.0, 1.0, 1.0, 1.0);"
"}";

void AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint shaderObj = glCreateShader(shaderType);
    if (shaderObj == 0)
    {
        fprintf(stderr, "Error creating shader object\n");
        throw std::runtime_error("Error creating shader object");
    }
    const GLchar* text[1];
    text[0] = shaderCode;
    GLint lengths[1];
    lengths[0] = strlen(shaderCode);
    glShaderSource(shaderObj,1,text,lengths);
    glCompileShader(shaderObj);
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderObj,1024,NULL,infoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
        throw std::runtime_error("Error compiling shader");
    }
    glAttachShader(shaderProgram, shaderObj);    
}

void CompileShaders()
{
    GLuint shaderProgram = glCreateProgram();
    if (shaderProgram == 0)
    {
        fprintf(stderr, "Error creating shader program\n");
        throw std::runtime_error("Error creating shader program");
    }

    AddShader(shaderProgram,vertexShader,GL_VERTEX_SHADER);
    AddShader(shaderProgram,fragmentShader,GL_FRAGMENT_SHADER);
    GLint success = 0;
    GLchar errorLog[1024] = { 0 };
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
        throw std::runtime_error("Error linking shader program");
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
        throw std::runtime_error("Invalid shader program");
    }

    glUseProgram(shaderProgram);
            
    gWorldLocation = glGetUniformLocation(shaderProgram, "gWorld");
    if (gWorldLocation == 0xFFFFFFFF)
        throw std::runtime_error("can't get gWorldLocation");
        
    //~ gSampler = glGetUniformLocation(shaderProgram, "gSampler");
    //~ assert(gSampler != 0xFFFFFFFF);    
}

#ifndef SGW_VIRTUAL_RENDERER_INTERFACE
void sgw::OpenGLRenderer::CheckFlushList(const BaseShape& shape)
{
    if (m_flushListCollection.empty() || m_flushListCollection.back().type != shape.GetType())
    {
        FlushList flushList;
        flushList.type = shape.GetFlushListType();
        m_flushListCollection.push_back(flushList);
    }
}

void sgw::OpenGLRenderer::ClearFlushListCollection()
{
    m_flushListCollection.clear();
}
#endif

void sgw::OpenGLRenderer::Render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
    glClear(GL_COLOR_BUFFER_BIT);
    
    unsigned int size = m_flushListCollection.size();
    DEBUG_PRINT("before render");
    for (unsigned int i = 0; i < size; i++)
    {
        const FlushList& flushList = m_flushListCollection[i];        
        const VerticesVector& vertices = flushList.vertices;
        const IndicesVector& indices = flushList.indices;
        DEBUG_PRINT("before gen buffers");
        glGenBuffers(1,&vertexBuffer);
        glGenBuffers(1,&indexBuffer);
        DEBUG_PRINT("before bind buffers");
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        DEBUG_PRINT("before buffer data");
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*3*4, &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*4, &indices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        DEBUG_PRINT("before setting primitive restart index");
        //glPrimitiveRestartIndex(15000);
        DEBUG_PRINT("before draw elements");
        glDrawElements(ConvertFlushListType(flushList.type), indices.size(), GL_UNSIGNED_INT, 0);
        //glDrawArrays(ConvertFlushListType(flushList.type), 0, 4);
        
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glDeleteBuffers(1,&vertexBuffer);
        glDeleteBuffers(1,&indexBuffer);
    }
    
    ClearFlushListCollection();
}

void sgw::OpenGLRenderer::Init(const AppData& appData)
{
    m_appData = appData;
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      throw std::runtime_error("couldn't init GLEW");
    }
    CompileShaders();
    perspectiveTransform.SetOpenGLOrthographicProjection(m_appData.windowSize.width-0.5, m_appData.windowSize.height-0.5, 1, -1);
    glUniformMatrix4fv(gWorldLocation,1,GL_TRUE,&perspectiveTransform.m_elements[0][0]);
}

void sgw::OpenGLRenderer::Draw(const BaseShape& shape)
{
    DEBUG_PRINT("draw shape");
    
    int shapeType = shape.GetType();
    CheckFlushList(shape);
    FlushList& flushList = m_flushListCollection.back();
    
    switch (shapeType)
    {
        case BaseShape::SHAPE_RECTANGLE:
        {
            const Rect& rect = static_cast<const Rect&>(shape);
            //construct primitive to draw, each position refers to a
            //point in the primitive (a rectangle)
            
            Vec3 vertices[4];

            vertices[0] = rect.GetTopLeft();
            
            vertices[1].x = rect.GetBottomRight().x;
            vertices[1].y = rect.GetTopLeft().y;
            vertices[1].z = 0;
            
            vertices[2] = rect.GetBottomRight();
            
            vertices[3].x = rect.GetTopLeft().x;
            vertices[3].y = rect.GetBottomRight().y;
            vertices[3].z = 0;
            
// if the rect is filled, add 1 to bottom right coordinates 
// to make sure that the bottom right vertex is actually drawn. 
// see https://msdn.microsoft.com/en-us/library/windows/desktop/bb147314%28v=vs.85%29.aspx
                       
            if (rect.IsFilled())
            {           
                vertices[1].x += 1;
                vertices[2].x += 1;
                vertices[2].y += 1;
                vertices[3].y += 1;
            }            
            
            
            unsigned int verticesSize = flushList.vertices.size();
            for (int i = 0; i < 4; i++)
            {
                flushList.vertices.push_back(vertices[i]);
            }
            if (rect.IsFilled())
            {
                // if the rect is filled, draw it as a list of triangles
                // every three points correspond to one triangle.
                flushList.indices.push_back(verticesSize);
                flushList.indices.push_back(verticesSize+1);
                flushList.indices.push_back(verticesSize+2);
                flushList.indices.push_back(verticesSize);
                flushList.indices.push_back(verticesSize+2);
                flushList.indices.push_back(verticesSize+3);    
            }
            else
            {
                // if the rect is not filled, draw it as a list of lines
                // I chose it instead of LINES_LOOP to be able to batch
                // several rectangles (with LINES_LOOP you need to stop
                // after each. so every two points correspond to one line
                flushList.indices.push_back(verticesSize);
                flushList.indices.push_back(verticesSize+1);
                flushList.indices.push_back(verticesSize);
                
                flushList.indices.push_back(verticesSize+1);
                flushList.indices.push_back(verticesSize+2);
                flushList.indices.push_back(verticesSize+1);
                
                flushList.indices.push_back(verticesSize+2);
                flushList.indices.push_back(verticesSize+3);
                flushList.indices.push_back(verticesSize+2);
                
                flushList.indices.push_back(verticesSize+3);
                flushList.indices.push_back(verticesSize);
                flushList.indices.push_back(verticesSize+3);    
            }
        }
        break;
    }
}
