#ifndef CHECK_HPP_INCLUDED
#define CHECK_HPP_INCLUDED

#include <sgw/sgw.hpp>
#include <functional>
#include <memory>

namespace test
{


class Check
{
public:
    Check(sgw::App& app);
    void RunTests();
    
protected:
    sgw::App& m_app;
    unsigned int CheckPixel(float x, float y);
    bool TestDraw(const char* testName, int shapeType, bool filled, int numRuns);    
    bool CheckDraw(const sgw::BaseShape& shape);
    std::unique_ptr<sgw::BaseShape> CreateRandomShape(int shapeType);
};


}

#endif
