



#include    "vector.h"
//#include    "list.h"
#include    <iostream>


int main(){
    tinySTL::vector<int> a;

    a.push_back(1);
    a.push_back(2);
    std::cout<<a[1]<<std::endl;
    std::cout<<a.size()<<"cap"<<a.capacity()<<std::endl;
    return 0;
}