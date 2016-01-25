#include "OpenGLRenderer.hpp"
#include "FlushList.hpp"

#include <renderer/shape/AllShapes.hpp>
#include <utils/utils.hpp>
#include <math/math.hpp>

#include <vector>
#include <stdio.h>

#include <GL/glew.h>

GLuint vertexBuffer = 0;
GLuint indexBuffer = 0;

GLuint primitiveRestartIndex = 65000;

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
        case sgw::FlushList::TYPE_RECTANGLES:
        case sgw::FlushList::TYPE_TRIANGLES:
            return GL_LINES;
            break;
            
        case sgw::FlushList::TYPE_LINE_STRIP:
            return GL_LINE_STRIP;
            break;
            
        case sgw::FlushList::TYPE_FILLED_TRIANGLES:
            return GL_TRIANGLES;
            break;
    }
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
      throw "couldn't init GLEW";
    }
    
    //~ m_transform.SetPerspectiveProjection(WINDOW_WIDTH,WINDOW_HEIGHT,DegreesToRadians(60.0f));
    //~ m_transform.SetCameraPosition(Vec3(0.0f, 0.0f, 0.0f));        
    //~ m_transform.SetCamera(m_camera);
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
            Vec3 vertices[4];
            vertices[0] = rect.GetTopLeft();
            
            vertices[1].x = rect.GetBottomRight().x;
            vertices[1].y = rect.GetTopLeft().y;
            vertices[1].z = 0;
            
            vertices[2] = rect.GetBottomRight();
            
            vertices[3].x = rect.GetTopLeft().x;
            vertices[3].y = rect.GetBottomRight().y;
            vertices[3].z = 0;
            unsigned int verticesSize = flushList.vertices.size();
            for (int i = 0; i < 4; i++)
            {
                vertices[i] = Utils::ClampVectorFromScreenSize(vertices[i],m_appData.windowSize,-1,1);
                vertices[i].y = -vertices[i].y;
                flushList.vertices.push_back(vertices[i]);
            }
            if (rect.IsFilled())
            {
                flushList.indices.push_back(verticesSize);
                flushList.indices.push_back(verticesSize+1);
                flushList.indices.push_back(verticesSize+2);
                flushList.indices.push_back(verticesSize);
                flushList.indices.push_back(verticesSize+2);
                flushList.indices.push_back(verticesSize+3);    
            }
            else
            {
                flushList.indices.push_back(verticesSize);
                flushList.indices.push_back(verticesSize+1);
                flushList.indices.push_back(verticesSize+1);
                flushList.indices.push_back(verticesSize+2);
                flushList.indices.push_back(verticesSize+2);
                flushList.indices.push_back(verticesSize+3);
                flushList.indices.push_back(verticesSize+3);
                flushList.indices.push_back(verticesSize);    
            }
        }
        break;
    }
}
