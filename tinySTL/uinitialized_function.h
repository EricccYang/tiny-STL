#ifndef _UNINITIALIZED_FUNCTION_H_
#define _UNINITIALIZED_FUNCTION_H_


#include "Iterator.h"
#include "Construct.h"
#include "Algorithm.h"

namespace tinySTL{
//---------------------------------------------
//uninitialized_copy

    template< class input_iterator, class forward_iterator, class T>
    inline forward_iterator _uninitialized_copy_aux(input_iterator first, input_iterator last,
                                                        forward_iterator result, _true_type ){
        return copy(first, last ,result);
    }

    template< class input_iterator, class forward_iterator, class T>
    inline forward_iterator _uninitialized_copy_aux(input_iterator first, input_iterator last,
                                                    forward_iterator result, _false_type ){
        forward_iterator cur= result;
        for(;first!=last;first++,cur++){
            construct(&*cur, *first);
        }
        return cur;
    }

//source function
    template <class input_iterator, class forward_iterator, class T>
    inline forward_iterator _uninitialized_copy(input_iterator first, input_iterator last, forward_iterator result, T*){
        typedef typename _type_traits<T>::is_POD_type is_POD_type;
        return _uninitialized_copy_aux(first, last, result, is_POD_type());

    }

//aux function for divide POD type
    template <class input_iterator, class forward_iterator>
    forward_iterator uninitialized_copy(input_iterator first, input_iterator last, forward_iterator result){
        _uninitialized_copy(first, last, result, value_type(result));
    }



//----------------------------------------------
//uninitialized_fill


    template< class forward_iterator, class T>
    void _uninitialzed_fill_aux(forward_iterator first, forward_iterator last, const T& x, _false_type){
        forward_iterator cur = first;
        for(;cur!=last;cur++){
            construct(&*cur, x);
        }
    }

    template< class forward_iterator, class T>
    void _uninitialzed_fill_aux(forward_iterator first, forward_iterator last, const T& x, _true_type){
        fill(first, last ,x );
    }


    template <class forward_iterator, class T, class T1>
    void unitialized_fill(forward_iterator first, forward_iterator last, const T& x, T1*){
        typedef typename _type_traits<forward_iterator>::is_POD_type is_POD_type;
        _uninitialized_fill_aux(first, last, x, is_POD_type);
    }


    template <class forward_iterator, class T>
    void uninitialized_fill(forward_iterator first, forward_iterator last, const T& x){
        _uninitialized_fill(first, last, x, value_type(first));
    }
//--------------------------------------------
// uninitialized_fill_n

    template<class forward_iterator, class Size, class T, class T1>
    inline forward_iterator _uninitialized_fill_n(forward_iterator first, Size n, const T& x, T1*){
        typedef typename _type_traits<T1>::is_POD_type;
        return _uninitialized_fill_n_aux(first, n, x, is_POD_type());
    }

    template <class forward_iterator, class Size, class T>
    inline forward_iterator uninitialized_fill_n(forward_iterator first, Size n, const T& x){
        return _uninitialized_fill_n(first, n, x, value_type(first));
    }

    template <class forward_iterator, class Size, class T>
    forward_iterator _uninitialized_fill_n_aux(forward_iterator first, Size n, const T& x,_true_type){
        return fill_n(first, n ,x);  //fill_n 交由位于Algorithm.h的高阶函数实现
    }

    template <class forward_iterator, class Size, class T>
    forward_iterator _uninitialized_fill_n_aux(forward_iterator first, Size n, const T& x,_false_type){
        for(;first!=last;first++){
            construct(first,x);
        }
    }






}

