#include <iostream>
#include <functional>
#include <tr1/unordered_map>
 
int main( int argc, char** argv )
{
    //using boost.tr1 library ( #include <boost/tr1/functional.hpp> )
    std::cout << std::tr1::hash<char*>()( argv[0] ) << '\n' ;

    return 0;
}
