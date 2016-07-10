#include "OpenGLRenderer.hpp"
#include "FlushList.hpp"

#include "Vertex.hpp"
#include <sgw/geom/AllShapes.hpp>
#include <utils/utils.hpp>
#include <utils/Logger.hpp>
#include <math/math.hpp>
#include <sgw/Texture.hpp>
#include <sgw/App.hpp>

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
GLuint gTextureType;

sgw::Mat4 perspectiveTransform;

sgw::OpenGLRenderer::OpenGLRenderer()
#ifdef SGW_VIRTUAL_RENDERER_INTERFACE
    : BaseRenderer()
#endif
{

}


static GLenum ConvertFlushListType(int type)
{
    sgw::Logger::FLogTrace("ConvertFlushListType");
    switch(type)
    {         
        case sgw::FlushList::TYPE_LINES:
        case sgw::FlushList::TYPE_TRIANGLES:
            sgw::Logger::FLogTrace("ConvertFlushListType -> return: GL_LINES");
            return GL_LINES;
            break;
            
        case sgw::FlushList::TYPE_LINE_LOOP:
            sgw::Logger::FLogTrace("ConvertFlushListType -> return: GL_LINES");
            return GL_LINES;
            break;
            
        case sgw::FlushList::TYPE_LINE_STRIP:
            sgw::Logger::FLogTrace("ConvertFlushListType -> return: GL_LINE_STRIP");
            return GL_LINE_STRIP;
            break;
            
        case sgw::FlushList::TYPE_FILLED_TRIANGLES:
        case sgw::FlushList::TYPE_RECTANGLES:
            sgw::Logger::FLogTrace("ConvertFlushListType -> return: GL_TRIANGLES");
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
"uniform int gTextureType;"
"in vec4 retClr;"
"in vec2 retTexCoord;"
"out vec4 fragColor;"
"vec4 endColor;"
"\n"
"void main()"
"{"
"       if (gTextureType > 0)"
"       {"
"           endColor = texture2D(gSampler,retTexCoord.xy);"
"           if (gTextureType == 4)"
"               endColor = vec4(endColor.rgb*retClr.rgb,endColor.a*retClr.a);"
"           else if (gTextureType == 3)"
"               endColor = vec4(endColor.rgb*retClr.rgb,1.0);"
"       }"
"       else"
"       {"
"           endColor = retClr;"
"       }"
"       fragColor = endColor;"
"}";

void AddShader(GLuint shaderProgram, const char* shaderCode, 
                                                GLenum shaderType)
{
    sgw::Logger::FLogTrace("OpenGLRenderer::AddShader, type: %d",shaderType);
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
    sgw::Logger::FLogTrace("OpenGLRenderer::CompileShaders");
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
    
    gTextureType = glGetUniformLocation(shaderProgram, "gTextureType");
    if (gWorldLocation == 0xFFFFFFFF)
        throw std::runtime_error("can't get gTextureType");
    glUniform1i(gTextureType, 0);
}

#ifndef SGW_VIRTUAL_RENDERER_INTERFACE
void sgw::OpenGLRenderer::CheckFlushList(const BaseShape& shape)
{
    Logger::FLogTrace("OpenGLRenderer::CheckFlushList");
    if (m_flushListCollection.empty() || 
        m_flushListCollection.back().type != shape.GetFlushListType() ||
        shape.GetTexture() != m_flushListCollection.back().texture)
    {
        FlushList flushList;
        flushList.type = shape.GetFlushListType();
        //flushList.textureType = shape.GetTextureType();
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
    sgw::Logger::FLogTrace("OpenGLRenderer::Render");
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
    //glClear(GL_COLOR_BUFFER_BIT);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDepthMask(GL_TRUE);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);    
        
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
            Logger::FLogInfo("Flushlist texture is: %x, noOfComponents: %d",flushList.texture, flushList.texture->GetNumberOfComponents());
            BindTexture(GL_TEXTURE0,*flushList.texture);
            glUniform1i(gTextureType, flushList.texture->GetNumberOfComponents());
        }
        else
        {
            Logger::FLogTrace("Flushlist texture is NULL");
            glUniform1i(gTextureType, 0);
        }
        sgw::Logger::FLogTrace("flushList.type: %d",flushList.type);
        sgw::Logger::FLogTrace(flushList.vertices);
        sgw::Logger::FLog(sgw::Logger::LOG_LEVEL_TRACE, indices);
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
    sgw::Logger::FLogTrace("OpenGLRenderer::Init");
    m_appData = appData;
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        throw std::runtime_error("couldn't init GLEW");
    }
    CompileShaders();
    //perspectiveTransform.SetPerspectiveProjection(m_appData.windowSize.width, m_appData.windowSize.height, -sgw::PI, 1, -1);
    float depth = (m_appData.windowSize.width+m_appData.windowSize.height)/2;
    perspectiveTransform.SetOpenGLOrthographicProjection(
        m_appData.windowSize.width, m_appData.windowSize.height, 
        depth/2, -depth/2);
        
    glUniformMatrix4fv(gWorldLocation,1,GL_TRUE,
        &perspectiveTransform.m_elements[0][0]);

    glEnable(GL_BLEND);    
    SetBlender(sgw::App::BLENDER_FUNC_ADD, sgw::App::BLENDER_ONE, 
        sgw::App::BLENDER_INVERSE_ALPHA);
          
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
}

void sgw::OpenGLRenderer::SetBlender(int func, int src, int dst)
{
    sgw::Logger::FLogTrace("OpenGLRenderer::SetBlender");
    switch(func)
    {
        case sgw::App::BLENDER_FUNC_ADD:
            func = GL_FUNC_ADD;
            break;
        
        case sgw::App::BLENDER_FUNC_SRC_MINUS_DST:
            func = GL_FUNC_SUBTRACT;
            break;
        
        case sgw::App::BLENDER_FUNC_DST_MINUS_SRC:
            func = GL_FUNC_REVERSE_SUBTRACT;
            break;            
    }
    
    switch(src)
    {
        case sgw::App::BLENDER_ONE:
            src = GL_ONE;
            break;
            
        case sgw::App::BLENDER_ALPHA:
            src = GL_SRC_ALPHA;
            break;
        
        case sgw::App::BLENDER_INVERSE_ALPHA:
            src = GL_ONE_MINUS_SRC_ALPHA;
            break;
    }
    
    switch(dst)
    {
        case sgw::App::BLENDER_ONE:
            dst = GL_ONE;
            break;
            
        case sgw::App::BLENDER_ALPHA:
            dst = GL_SRC_ALPHA;
            break;
        
        case sgw::App::BLENDER_INVERSE_ALPHA:
            dst = GL_ONE_MINUS_SRC_ALPHA;
            break;
    }    
    
    glBlendEquation(func);
    glBlendFunc(src, dst);    
}

void sgw::OpenGLRenderer::Draw(const BaseShape& shape)
{
    sgw::Logger::FLogTrace("OpenGLRenderer::Draw");
    
    int shapeType = shape.GetType();
    CheckFlushList(shape);
    FlushList& flushList = m_flushListCollection.back();
    const Texture* const texture = shape.GetTexture();
    
    switch (shapeType)
    {
        case BaseShape::SHAPE_RECTANGLE:
        case BaseShape::SHAPE_IMAGE:
        {
            Logger::FLogDebug("-----Drawing a rect or an image");
            const Rect& rect = static_cast<const Rect&>(shape);
            //construct the primitive to draw; each vertex refers to a
            //point in the primitive (a rectangle)        
            std::vector<Vertex> vertices = rect.GetTransform().Apply(rect.GetVertices());
            
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
            //sgw::Logger::FLogTrace("pos 0 x = %.1f",vertices[0].pos.x);     
            
            //Logger::FLog(Logger::LOG_LEVEL_INFO,flushList.indices);
            unsigned int verticesSize = flushList.vertices.size();
            for (int i = 0; i < 4; i++)
            {
                vertices[i].clr = rect.GetColor();
            }
            flushList.vertices.insert(flushList.vertices.end(),vertices.begin(),vertices.end());
            sgw::Logger::FLog(sgw::Logger::LOG_LEVEL_DEBUG,"Rect vertices:");
            sgw::Logger::FLog(sgw::Logger::LOG_LEVEL_DEBUG, vertices);
            if (rect.IsFilled())
            {
                // if the rect is filled, draw it as a list of triangles
                // every three points correspond to one triangle.
                flushList.indices.push_back(verticesSize);
                flushList.indices.push_back(verticesSize+2);
                flushList.indices.push_back(verticesSize+1);
                
                flushList.indices.push_back(verticesSize);
                flushList.indices.push_back(verticesSize+3); 
                flushList.indices.push_back(verticesSize+2);
            }
            else
            {
                // if the rect is not filled, draw it as a list of lines
                // I chose it instead of LINES_LOOP to be able to batch
                // several rectangles (with LINES_LOOP you need to stop
                // after each or use glPrimitiveRestartIndex, which is
                // OpenGL 3.1 and higher, and sgw supposed to support 2.0) 
                // So every two points correspond to one line
                
                flushList.indices.push_back(verticesSize);
                flushList.indices.push_back(verticesSize+1);
                
                flushList.indices.push_back(verticesSize+1);
                flushList.indices.push_back(verticesSize+2);
                
                flushList.indices.push_back(verticesSize+3);
                flushList.indices.push_back(verticesSize+2);
                
                flushList.indices.push_back(verticesSize);
                flushList.indices.push_back(verticesSize+3);
                
                
                //~ flushList.indices.push_back(verticesSize);
                //~ flushList.indices.push_back(verticesSize+1);
                //~ flushList.indices.push_back(verticesSize);
                
                //~ flushList.indices.push_back(verticesSize+1);
                //~ flushList.indices.push_back(verticesSize+2);
                //~ flushList.indices.push_back(verticesSize+1);
                
                //~ flushList.indices.push_back(verticesSize+2);
                //~ flushList.indices.push_back(verticesSize+3);
                //~ flushList.indices.push_back(verticesSize+2);
                
                //~ flushList.indices.push_back(verticesSize+3);
                //~ flushList.indices.push_back(verticesSize); 
                //~ flushList.indices.push_back(verticesSize+3);                
            }
        }
        break;
        
        case BaseShape::SHAPE_TRIANGLE:
        case BaseShape::SHAPE_OCTAHEDRON:
        case BaseShape::SHAPE_SPHERE:
        {
            sgw::Logger::FLogDebug("------Drawing triangles");
            //const Triangle& tr = static_cast<const Triangle&>(shape);
            
            //Logger::FLogTrace("Draw : triangle points before transform:");
            //Logger::FLog(Logger::LOG_LEVEL_TRACE, points);
            std::vector<Vertex> vertices = shape.GetTransform().Apply(shape.GetVertices());
            //Logger::FLogDebug("Draw : triangle vertices after transform:");
            //Logger::FLog(Logger::LOG_LEVEL_DEBUG, vertices);
            const int sz = vertices.size();
            //Logger::FLogInfo("Triangles vertices:");
            //Logger::FLog(Logger::LOG_LEVEL_INFO, vertices);
            const unsigned int verticesSize = flushList.vertices.size();
            
            
            for (int i = 0; i < sz; i++)
            {
                vertices[i].clr = shape.GetColor();
                //~ if (shapeType == BaseShape::SHAPE_SPHERE && texture) 
                //~ {
                    //~ if (vertices[i].texCoord.x == 1.0f)
                        //~ vertices[i].clr = 0x0000FFFF;
                //~ }
                //flushList.vertices.push_back(vertices[i]);
                if (shape.IsFilled()) 
                {
                    //Logger::FLogInfo("filled");
                    flushList.indices.push_back(verticesSize+i);
                }
                else
                {
                    Logger::FLogInfo("not filled");
                    if ((i+1) % 3 != 0)
                    {
                        flushList.indices.push_back(verticesSize+i);
                        flushList.indices.push_back(verticesSize+i+1);
                    }
                    else
                    {
                        flushList.indices.push_back(verticesSize+i);
                        flushList.indices.push_back(verticesSize+i-2);
                    }               
                }
            }
            //Logger::FLog(Logger::LOG_LEVEL_INFO,flushList.indices);
            flushList.vertices.insert(flushList.vertices.end(),vertices.begin(),vertices.end());
            //Logger::FLogInfo(flushList.vertices);                  
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
