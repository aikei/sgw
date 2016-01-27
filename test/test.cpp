#include <sgw/sgw.hpp>
#include "check.hpp"

int main(int argc, char** argv)
{   
    sgw::App app(800,600);
    test::Check check(app); 
    
    check.RunTests();
}
