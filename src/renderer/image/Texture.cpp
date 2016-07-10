#include <sgw/Texture.hpp>
#include <sgw/App.hpp>
#include <utils/Logger.hpp>
#include <app/BaseApp.hpp>
#include <png.h>
#include <cstdio>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <GL/glew.h>

sgw::Texture::Texture(const char* fileName)
{
    FileLoad(fileName);
    GPULoad();
}

void sgw::Texture::FileLoad(const char* fileName)
{
    Logger::FLog(Logger::LOG_LEVEL_TRACE,"Loading file");
    FILE* f = fopen(fileName, "rb");
    if (!f)
    {
        Logger::FLog(Logger::LOG_LEVEL_ERROR,"Can't open file %s",fileName);
        throw std::runtime_error("Can't open file");
    }
    png_bytep header = (png_bytep)malloc(8);
    if (!fread(header, 1, 8, f))
        Logger::FLog(Logger::LOG_LEVEL_ERROR,"Nothing read from file %s",fileName);
    bool is_png = !png_sig_cmp(header, 0, 8);
    if (!is_png)
    {
        Logger::FLog(Logger::LOG_LEVEL_ERROR,"File %s is not .png",fileName);
    }
    free(header);
    
    Logger::FLog(Logger::LOG_LEVEL_TRACE,"File %s is .png",fileName);
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
        NULL, NULL, NULL);
    if (!png_ptr)
    {
        Logger::FLog(Logger::LOG_LEVEL_ERROR,"Couldn't create a png read struct!");
        throw std::runtime_error("Couldn't create a png read struct!");
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr,
           (png_infopp)NULL, (png_infopp)NULL);
        throw std::runtime_error("Couldn't create a png info struct!");
    }
    
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr,
          (png_infopp)NULL);
        throw std::runtime_error("Couldn't an end info struct for png!");
    }
    
    png_init_io(png_ptr, f);
    png_set_sig_bytes(png_ptr, 8);
    
    png_read_info(png_ptr, info_ptr);
    
    int bit_depth, color_type, interlace_type, compression_type, filter_method;
    
    png_get_IHDR(png_ptr, info_ptr, &m_width, &m_height,
       &bit_depth, &color_type, &interlace_type,
       &compression_type, &filter_method);    
    Logger::FLog(Logger::LOG_LEVEL_TRACE,
        "File width: %d, height: %d, depth: %d, color type: %d",
        m_width,m_height,bit_depth,color_type);
    if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) 
    {
        m_numberOfComponents = 4;
        Logger::FLog(Logger::LOG_LEVEL_TRACE, "Color type is PNG_COLOR_TYPE_RGB_ALPHA");
    } 
    else if (color_type == PNG_COLOR_TYPE_RGB)
    {
        m_numberOfComponents = 3;
        Logger::FLog(Logger::LOG_LEVEL_TRACE, "Color type is PNG_COLOR_TYPE_RGB");
    }
    Logger::FLog(Logger::LOG_LEVEL_TRACE, "Interlacing: %d", interlace_type);
    
    png_read_update_info(png_ptr, info_ptr);
    
    m_pImageData = new unsigned char[m_width*m_height*m_numberOfComponents];
    
    unsigned char** rowspp = new unsigned char*[m_height];
    for (int i = 0; i < m_height; i++)
        rowspp[m_height-1-i] = m_pImageData+i*m_width*m_numberOfComponents;   
    png_read_image(png_ptr, rowspp);
    
    //premultiply alpha
    if (sgw::App::PremultiplyAlpha && m_numberOfComponents == 4)
    {
        for (int i = 0; i < m_width*m_height*4; i += 4)
        {
            float a = m_pImageData[i+3]/255.0f;
            m_pImageData[i] *= a;
            m_pImageData[i+1] *= a;
            m_pImageData[i+2] *= a;
        }
    }
    delete[] rowspp;
    //~ for (int i = 0; i < m_width*m_height*4; i+=4)
    //~ {
        //~ int n = 0;
        //~ memcpy(&n,&ch[i],4);
        //~ Logger::FLogNONL("%X ",n);
        //~ if (i % (m_width*4) == 0)
            //~ Logger::FLogNONL("\n");
    //~ }
    
    png_read_end(png_ptr, end_info);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);    
}

#ifdef SGW_RENDERER_OPENGL

void sgw::Texture::GPULoad()
{
    glGenTextures(1, &glTextureObj);
    glBindTexture(GL_TEXTURE_2D, glTextureObj);
    if (m_numberOfComponents == 4) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, 
            GL_RGBA, GL_UNSIGNED_BYTE, m_pImageData);
    }
    else if (m_numberOfComponents == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, 
            GL_RGB, GL_UNSIGNED_BYTE, m_pImageData);        
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    glBindTexture(GL_TEXTURE_2D, 0);    
}

void sgw::Texture::GPUDelete()
{
    glDeleteTextures(1, &glTextureObj);
}

#elif defined SGW_VIRTUAL_RENDERER_INTERFACE

void sgw::Texture::GPULoad()
{
    if (sgw::BaseApp::VideoBackend == SGW_CONST_RENDERER_BACKEND_OPEN_GL)
    {
        glGenTextures(1, &texture.glTextureObj);
        glBindTexture(GL_TEXTURE_2D, texture.glTextureObj);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, 
            GL_RGBA, GL_UNSIGNED_BYTE, m_pImageData);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void sgw::Texture::GPUDelete()
{
    if (sgw::BaseApp::VideoBackend == SGW_CONST_RENDERER_BACKEND_OPEN_GL)
    {
        glDeleteTextures(1, &texture.glTextureObj);
    }
}

#endif

sgw::Texture::~Texture()
{
    GPUDelete();
    delete[] m_pImageData;
}
