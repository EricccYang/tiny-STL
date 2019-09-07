#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_


#include "Alloc.h"
#include "Construct.h"



namespace tinySTL{



    //一个接口
    template<class T, class Alloc=alloc>
    class   simple_alloc{







    public:
        static T* allocate(size_t n){
            return 0==n ? 0 :(T*) Alloc::allocate(n* sizeof(T));
        }

        static T* allocate(void){
            return (T*) Alloc::allocate(sizeof(T));
        }

        static void deallocate(T* p, size_t n){
            if(n!=0)
                Alloc::deallocate(p, n* sizeof(T));
        }

        static void deallocate(T* p){
            Alloc::deallocate(p, sizeof(T));
        }
    };






}


#endif

