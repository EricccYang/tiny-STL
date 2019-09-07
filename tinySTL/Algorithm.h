#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_



#include "Iterator.h"
#include "Type_Traits.h"
#include <cstring>

namespace tinySTL {


//-------copy function----------------------
    template<class T>
    inline T *copy_t(const T *first, const T *last, T *result, _true_type) {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }

    template<class T>
    inline T *copy_t(const T *first, const T *last, T *result, _false_type) {
        return copy_d(first, last, result, (ptrdiff_t *) 0);
    }

    /*
    template<class input_iterator, class output_iterator>
    struct _copy_dispatch{
        output_iterator operator()(input_iterator first, input_iterator last, output_iterator result){
            return _copy(first, last, result, iterator_category(first));
        }
    };
     */

    template<class random_access_iterator, class output_iterator,  class Distance>
    inline output_iterator
    copy_d(random_access_iterator first, random_access_iterator last, output_iterator result, Distance *) {
        for (Distance n = last - first; n > 0; --n, ++result, ++first) {
            *result = *first;
        }
        return result;
    }

    template<class input_iterator, class output_iterator>
    inline output_iterator
    _copy(input_iterator first, input_iterator last, output_iterator result, input_iterator_tag) {
        for (; first != last; ++result, ++first) {
            *result = *first;
        }
        return result;
    }

    template<class random_access_iterator, class output_iterator>
    inline output_iterator _copy(random_access_iterator first, random_access_iterator last, output_iterator result,
                                 random_access_iterator_tag) {
        return copy_d(first, last, result, difference_type(first));
    }

    template<class input_iterator, class output_iterator>
    struct copy_dispatch {
        output_iterator operator()(input_iterator first, input_iterator last, output_iterator result) {
            return _copy(first, last, result, iterator_category(first));
        }
    };

    template<class T>
    struct copy_dispatch<T *, T *> {
        T *operator()(T *first, T *last, T *result) {
            typedef typename _type_traits<T>::has_trivial_assignment_operator t;
            return copy_t(first, last, result, t());
        }
    };

    template<class T>
    struct copy_dispatch<const T *, T *> {
        T *operator()(const T *first, const T *last, T *result) {
            typedef typename _type_traits<T>::has_trivial_assignment_operator t;
            return copy_t(first, last, result, t());
        }
    };

    template<class input_iterator, class output_iterator>
    inline output_iterator copy(input_iterator first, input_iterator last, output_iterator result) {
        return copy_dispatch<input_iterator, output_iterator>()
                (first, last, result);
    }

    inline char* copy(const char *first, const char *last, char *result) {
        memmove(result, first, last - first);
        return result + (last - first);
    }

    inline wchar_t* copy(const wchar_t *first, const wchar_t *last, wchar_t *result) {
        memmove(result, first, last - first);
        return result + (last - first);
    }
    //---------------end of copy function----------------




//-----------fill function---------------------------

    template<class forward_iterator, class T>
    inline void fill(forward_iterator first, forward_iterator last, const T &x) {
        for (; first != last; first++) {
            *first = x;
        }
    }

    //--------------------end of fill function----------------




//------------------fill_n  function-----------------
    template<class output_iterator, class Size, class T>
    output_iterator fill_n(output_iterator first, Size n, const T &x) {

        for (; n > 0; n--, ++first) {
            *first = x;
        }
        return first;
    }
    //--------------------end of fill_n function-------------






//-----------copyback------------------------
    template<class bidirectional_iterator1, class bidirectional_iterator2>
    inline bidirectional_iterator2 copy_backward(bidirectional_iterator1 first, bidirectional_iterator1 last,
                                                 bidirectional_iterator2 result) {
        for(;last!=first;last--,result--){
            *result = *last;
        }
        return result;
    }




    //------------end of copy_backward function----------


//----------uninitialized copy------------

    template<class input_iterator, class output_iterator>
    output_iterator uninitialized_copy(input_iterator first, input_iterator last, output_iterator result){

        for(;first!=last;first++,result++)
            *result= *first;
        return result;
    }



    //------------end of unitialized copy


}
#endif
