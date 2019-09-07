#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_



#include <new>
#include "Type_Traits.h"



//construct and destroy

namespace tinySTL {

//from helper function: non-trivial, destroy the element one by one , productivity is low
    template<class forward_iterator>
    inline void _destroy_aux(forward_iterator first, forward_iterator last, _false_type) {
        for (; first < last; first++) {
            destroy(&*first);
        }
    }
//from helper function: trivial type, do NOT need to destroy one by one
    template<class forward_iterator>
    inline void _destroy_aux(forward_iterator first, forward_iterator last, _true_type) {}

    /*
//helper function, whether it is a trivail type
    template<class forwardIterator, class T>
    inline void _destroy(forwardIterator first, forwardIterator last, T *pointer) {

        _destroy_aux(first, last, trivial_destructor());
    }

     */

//---------------------------------------------
    template<class T1, class T2>
    inline void construct(T1* p, const T2 &value) {
        new (p) T1(value);
    }   //调用T1::T1(value);

//destroy
    template<class T1>
    inline void destroy(T1 *p) { p->~T(); }

//destory second version,
    template<class forward_iterator>
    inline void destroy(forward_iterator first, forward_iterator last) {
        typedef typename _type_traits<forward_iterator>::is_POD_type is_POD_type;
        _destroy_aux(first, last, is_POD_type());
    }



//inline void destroy(char *, char *) {}
//inline void destroy(wchar_t *, wchar_t *){}


}
#endif
