#ifndef SGW_FLUSH_LIST_HPP
#define SGW_FLUSH_LIST_HPP

#include <vector>

namespace sgw
{


struct Vec3;

typedef std::vector<Vec3> VerticesVector;
typedef std::vector<unsigned int> IndicesVector;

struct FlushList
{
	int type;
	VerticesVector vertices;
	IndicesVector indices;
	
	enum
	{
		TYPE_LINES = 0,
		TYPE_LINE_STRIP = 1,
		TYPE_LINE_LOOP = 2,
		TYPE_TRIANGLES = 3,
		TYPE_RECTANGLES = 4,
		TYPE_FILLED_TRIANGLES = 5
	};
};


}

#endif
