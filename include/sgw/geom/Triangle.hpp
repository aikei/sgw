#ifndef SGW_TRIANGLE_INCLUDED
#define SGW_TRIANGLE_INCLUDED

#include <renderer/shape/BaseShape.hpp>
#include <renderer/FlushList.hpp>
#include <vector>

namespace sgw
{
    

class Triangle : public BaseShape
{
public:

    Triangle(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y)
    {
        InitTriangle(Vec3(p1x, p1y), Vec3(p2x, p2y), Vec3(p3x, p3y));
    }
    
    Triangle(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z)
    {
        InitTriangle(Vec3(p1x, p1y, p1z), Vec3(p2x, p2y, p2z), Vec3(p3x, p3y, p3z));
    }    
    
    Triangle(const Vec3& vertex1, const Vec3& vertex2, const Vec3& vertex3)
    {
        InitTriangle(vertex1, vertex2, vertex3);
    }
    
    inline void SetFilled(bool filled) 
    { 
        BaseShape::SetFilled(filled);
        if (m_bFilled)
            m_flushListType = FlushList::TYPE_FILLED_TRIANGLES;
        else
            m_flushListType = FlushList::TYPE_TRIANGLES;
    }
    
    inline const std::vector<Vec3>& GetPoints() const
    {
        return m_points;
    }
    
    std::vector<Triangle> Split() const;
    
    //~ inline void SetWidth(float newWidth)
    //~ {
        //~ m_size.width = newWidth;
    //~ }
    
    //~ inline void SetHeight(float newHeight)
    //~ {
        //~ m_size.height = newHeight;
    //~ }
    
    //~ inline void SetSize(const Size& newSize)
    //~ {
        //~ m_size = newSize;
    //~ }

protected:

    std::vector<Vec3> m_points;

    inline void InitTriangle(const Vec3& vertex1, const Vec3& vertex2, const Vec3& vertex3)
    {
        m_type = SHAPE_TRIANGLE;
        //m_pos = vertex1;
        Vec3 array[] = { vertex1, vertex2, vertex3 };
        m_points.insert(m_points.begin(), array, array+3);
    }
};


}

#endif

