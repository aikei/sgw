#include "OpenGLRenderer.hpp"
#include "FlushList.hpp"

#include "Vertex.hpp"
#include <sgw/geom/AllShapes.hpp>
#include <utils/utils.hpp>
#include <utils/Logger.hpp>
#include <math/math.hpp>
#include <sgw/Texture.hpp>

#include <cstring>
#include <vector>
#include <stdio.h>
#include <stdexcept>

#include <GL/glew.h>

GLuint vertexBuffer = 0;
GLuint indexBuffer = 0;

GLuint primitiveRestartIndex = 65000;
GLuint gWorldLocation;
GLuint gSampler;
GLuint gUseTexture;

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
"layout (location = 1) in vec4 clr;"
"layout (location = 2) in vec2 texCoord;"
"\n"
"uniform mat4 gWorld = mat4(1.0);"
"out vec4 retClr;"
"out vec2 retTexCoord;"
"\n"
"void main()"
"{"
"   gl_Position = gWorld*vec4(position, 1.0);"
"   retClr = clr;"
"   retTexCoord = texCoord;"     
"}";

const char* fragmentShader =
"#version 330\n"
"uniform sampler2D gSampler;"
"uniform bool gUseTexture;"
"in vec4 retClr;"
"in vec2 retTexCoord;"
"out vec4 fragColor;"
"vec4 endColor;"
"\n"
"void main()"
"{"
"       if (gUseTexture)"
"       {"
"           endColor = texture2D(gSampler,retTexCoord.xy);"
"           endColor = vec4(endColor.rgb*retClr.rgb,endColor.a*retClr.a);"
"       }"
"       else"
"           endColor = retClr;"
"       fragColor = endColor;"
"}";

void AddShader(GLuint shaderProgram, const char* shaderCode, 
                                                GLenum shaderType)
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
        throw std::runtime_error(infoLog);
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
        throw std::runtime_error(errorLog);
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
        throw std::runtime_error(errorLog);
    }

    glUseProgram(shaderProgram);
            
    gWorldLocation = glGetUniformLocation(shaderProgram, "gWorld");
    if (gWorldLocation == 0xFFFFFFFF)
        throw std::runtime_error("can't get gWorldLocation");
        
    gSampler = glGetUniformLocation(shaderProgram, "gSampler");
    if (gSampler == 0xFFFFFFFF)
        throw std::runtime_error("can't get gSampler!");
        
    glUniform1i(gSampler, 0);
    
    gUseTexture = glGetUniformLocation(shaderProgram, "gUseTexture");
    if (gWorldLocation == 0xFFFFFFFF)
        throw std::runtime_error("can't get gUseTexture");
    glUniform1i(gUseTexture, false);
}

#ifndef SGW_VIRTUAL_RENDERER_INTERFACE
void sgw::OpenGLRenderer::CheckFlushList(const BaseShape& shape)
{
    if (m_flushListCollection.empty() || 
        m_flushListCollection.back().type != shape.GetFlushListType() ||
        shape.GetTexture() != m_flushListCollection.back().texture)
    {
        FlushList flushList;
        flushList.type = shape.GetFlushListType();
        flushList.texture = shape.GetTexture();
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
        
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(sgw::Vertex), 
            &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*4, 
            &indices[0], GL_STATIC_DRAW);
            
        //position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                            sizeof(sgw::Vertex), 0);
        
        //clr
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 
                            sizeof(sgw::Vertex), (const GLvoid*)12);
                            
        //texture coordinate
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 
                            sizeof(sgw::Vertex), (const GLvoid*)16);      
        
        DEBUG_PRINT("before setting primitive restart index");
        //glPrimitiveRestartIndex(15000);
        DEBUG_PRINT("before draw elements");
        
        if (flushList.texture)
        {
            BindTexture(GL_TEXTURE0,*flushList.texture);
            glUniform1i(gUseTexture, true);
        }
        else
        {
            glUniform1i(gUseTexture, false);
        }
        
        glDrawElements(ConvertFlushListType(flushList.type), 
            indices.size(), GL_UNSIGNED_INT, 0);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glDeleteBuffers(1,&vertexBuffer);
        glDeleteBuffers(1,&indexBuffer);
        UnbindTexture();
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
    perspectiveTransform.SetOpenGLOrthographicProjection(
        m_appData.windowSize.width, m_appData.windowSize.height, 
        1, -1);
        
    glUniformMatrix4fv(gWorldLocation,1,GL_TRUE,
        &perspectiveTransform.m_elements[0][0]);
    
    //~ glEnable(GL_BLEND);
    //~ glBlendEquation(GL_FUNC_ADD);
    //~ glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
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
        case BaseShape::SHAPE_IMAGE:
        {
            const Rect& rect = static_cast<const Rect&>(shape);
            //construct the primitive to draw, each vertex refers to a
            //point in the primitive (a rectangle)
            
            Vertex vertices[4];

            vertices[0].pos = rect.GetPos();
            vertices[0].pos.z = 0;
            
            vertices[1].pos.x = rect.GetPos().x+rect.GetSize().width;
            vertices[1].pos.y = rect.GetPos().y;
            vertices[1].pos.z = 0;
            
            vertices[2].pos = rect.GetPos()+rect.GetSize();         
            vertices[2].pos.z = 0;
            
            vertices[3].pos.x = rect.GetPos().x;
            vertices[3].pos.y = rect.GetPos().y+rect.GetSize().height;
            vertices[3].pos.z = 0;
            printf("x, y, w, h = %.1f %.1f %.1f %.1f",
                rect.GetPos().x, rect.GetPos().y, rect.GetSize().width, rect.GetSize().height);
// if the rect is not filled, subtract 1 from bottom right coordinates 
// to make sure that the non-filled rect is drawn consistently with the
// filled one. 
// see https://msdn.microsoft.com/en-us/library/windows/desktop/bb147314%28v=vs.85%29.aspx
                       
            if (!rect.IsFilled())
            {           
                vertices[1].pos.x -= 1.0f;
                vertices[2].pos.x -= 1.0f;
                vertices[2].pos.y -= 1.0f;
                vertices[3].pos.y -= 1.0f;
            }            
            sgw::Logger::FLogTrace("pos 0 x = %.1f",vertices[0].pos.x);
            const Texture* const texture = rect.GetTexture();
            if (texture)
            {
                vertices[0].texCoord.x = 0.0f;
                vertices[0].texCoord.y = 1.0f;
                
                vertices[1].texCoord.x = 1.0f;
                vertices[1].texCoord.y = 1.0f;
                
                vertices[2].texCoord.x = 1.0f;
                vertices[2].texCoord.y = 0.0f;
                
                vertices[3].texCoord.x = 0.0f;
                vertices[3].texCoord.y = 0.0f;
            }            
            
            
            unsigned int verticesSize = flushList.vertices.size();
            for (int i = 0; i < 4; i++)
            {
                vertices[i].clr = rect.GetColor();
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
                
                //~ flushList.indices.push_back(verticesSize);
                //~ flushList.indices.push_back(verticesSize+1);
                
                //~ flushList.indices.push_back(verticesSize+1);
                //~ flushList.indices.push_back(verticesSize+2);
                
                //~ flushList.indices.push_back(verticesSize+3);
                //~ flushList.indices.push_back(verticesSize+2);
                
                //~ flushList.indices.push_back(verticesSize);
                //~ flushList.indices.push_back(verticesSize+3);
                
                
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

void sgw::OpenGLRenderer::UnbindTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void sgw::OpenGLRenderer::BindTexture(GLenum textureUnit, const sgw::Texture& texture)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture.glTextureObj);    
}
