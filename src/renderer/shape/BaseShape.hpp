#ifndef SGW_BASE_SHAPE_INCLUDED
#define SGW_BASE_SHAPE_INCLUDED

#include <math/math.hpp>
#include <utils/Color.hpp>
#include <renderer/FlushList.hpp>

namespace sgw
{
	
class BaseShape
{
public:

	enum Shape
	{
		SHAPE_RECTANGLE = 0
	};
	
	BaseShape()
	{
		m_outlineThickness = 1.0f;
		m_bFilled = false;
	}	
	
	inline void SetOutlineColor(const Color& _color) { m_outlineColor = _color; }	
	inline const Color& GetOutlineColor() const { return m_outlineColor; }
	inline void SetFillColor(const Color& _color) { m_fillColor = _color; }	
	inline const Color& GetFillColor() const { return m_fillColor; }	
	inline void SetOutlineThickness(float thickness) { m_outlineThickness = thickness; }
	inline float GetOutlineThickness() const { return m_outlineThickness; }
	inline const Size& GetSize() { return m_size; }
	inline virtual void SetFilled(bool filled) { m_bFilled = filled; }
	inline bool IsFilled() const { return m_bFilled; }
	inline int GetType() const { return m_type; }
	inline int GetFlushListType() const { return m_flushListType; }
	//virtual VerticesVector GetVertices()=0;
	//virtual IndicesVector GetIndices()=0;
	
protected:
	int m_flushListType;
	int m_type;
	Size m_size;
	Color m_outlineColor;
	Color m_fillColor;
	float m_outlineThickness;
	bool m_bFilled;
	//VerticesVector m_vertices;
	//IndicesVector m_indices;
};


}

#endif
