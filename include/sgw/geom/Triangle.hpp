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

    /**
     * \brief Constructs a triangle from a set of 2D coordinates
     * 
     * @param p1x x-coordinate of the first vertex of the triangle
     * @param p1y y-coordinate of the first vertex of the triangle
     * @param p2x x-coordinate of the second vertex of the triangle
     * @param p2y y-coordinate of the second vertex of the triangle
     * @param p3x x-coordinate of the third vertex of the triangle
     * @param p3y y-coordinate of the third vertex of the triangle
     */
    Triangle(float p1x, float p1y, float p2x, float p2y, float p3x, float p3y)
    {
        m_type = SHAPE_TRIANGLE;
        InitTriangle(Vec3(p1x, p1y), Vec3(p2x, p2y), Vec3(p3x, p3y));
    }
    
    /**
     * \brief Constructs a triangle from a set of 3D coordinates
     * 
     * @param p1x x-coordinate of the first vertex of the triangle
     * @param p1y y-coordinate of the first vertex of the triangle
     * @param p1z z-coordinate of the first vertex of the triangle
     * @param p2x x-coordinate of the second vertex of the triangle
     * @param p2y y-coordinate of the second vertex of the triangle
     * @param p2z z-coordinate of the second vertex of the triangle
     * @param p3x x-coordinate of the third vertex of the triangle
     * @param p3y y-coordinate of the third vertex of the triangle
     * @param p3z z-coordinate of the third vertex of the triangle
     */    
    Triangle(float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float p3x, float p3y, float p3z)
    {
        m_type = SHAPE_TRIANGLE;
        InitTriangle(Vec3(p1x, p1y, p1z), Vec3(p2x, p2y, p2z), Vec3(p3x, p3y, p3z));
    }
    
    /**
     * \brief Constructs a triangle from a set of `Vec3` objects
     * 
     * @param vertex1 first vertex of the triangle
     * @param vertex2 second vertex of the triangle
     * @param vertex3 third vertex of the triangle
     */     
    Triangle(const Vec3& vertex1, const Vec3& vertex2, const Vec3& vertex3)
    {
        m_type = SHAPE_TRIANGLE;
        InitTriangle(vertex1, vertex2, vertex3);
    }
    
    /**
     * \brief Set whether this triangle should be drawn filled or not
     * 
     * @param filled *true* for a filled and *false* (default) for a non-filled triangle
     */
    inline void SetFilled(bool filled) 
    { 
        BaseShape::SetFilled(filled);
        if (m_bFilled)
            m_flushListType = FlushList::TYPE_FILLED_TRIANGLES;
        else
            m_flushListType = FlushList::TYPE_TRIANGLES;
    }
    
    /**
     * \brief Splits this triangle into four equal triangles using points in the center
     * of the triangle's sides
     * 
     * @return A vector containing 4 new triangles
     */    
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

    void InitTriangle(const Vec3& vertex1, const Vec3& vertex2, const Vec3& vertex3);
    
};


}

#endif

