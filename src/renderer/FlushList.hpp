#ifndef SGW_FLUSH_LIST_HPP
#define SGW_FLUSH_LIST_HPP

#include <vector>
#include "Vertex.hpp"

namespace sgw
{


struct Vec3;
class Texture;

typedef std::vector<Vertex> VerticesVector;
typedef std::vector<unsigned int> IndicesVector;

struct FlushList
{
    int type, textureType;
    VerticesVector vertices;
    IndicesVector indices;
    const Texture* texture;
    
    enum
    {
        TYPE_LINES = 0,
        TYPE_LINE_STRIP = 1,
        TYPE_LINE_LOOP = 2,
        TYPE_TRIANGLES = 3,
        TYPE_RECTANGLES = 4,
        TYPE_FILLED_TRIANGLES = 5
    };
    
    FlushList()
    {
        texture = NULL;
    }
};


}

#endif
