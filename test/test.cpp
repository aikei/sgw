#include <sgw/sgw.hpp>
#include "check.hpp"
#include <cstdio>

int main(int argc, char** argv)
{   
    sgw::App app(1024,768);
    test::Check check(app); 
    
    check.RunTests();
}
