
#ifndef _VECTOR_H_
#define _VECTOR_H_





#include "Allocator.h"
#include "Iterator.h"
#include "Algorithm.h"


namespace tinySTL{

    template<class T, class Alloc = alloc>
    class vector{
        public:
            typedef T           value_type;
            typedef T*          pointer;
            typedef const T*    const_iterator;
            typedef T*          iterator;
            typedef T&          reference;
            typedef size_t      size_type;
            typedef ptrdiff_t   difference_type;    

        protected:

            typedef simple_alloc<value_type, Alloc>  data_allocator;
            
            iterator start;
            iterator finish;
            iterator end_of_storage;



        public:
            iterator begin() {   return start; }
            const_iterator  begin() const { return start;}
            iterator end()   {   return finish; }
            const_iterator  end() const {return finish;}
            
            void push_back(const T& x){
                if(finish!=end_of_storage){
                    construct(finish,x);
                }
                else{
                    insert_aux(end(),x);
                }
            }


            size_type size() const  {
                return  size_type ( end() - begin()) ;
            }

            size_type capacity() const {
                return (size_type(end_of_storage-begin()));
            }

            bool empty() const {
                return begin()==end();
            }

            reference operator[](size_type n){
                return *(begin()+n);
            }

    //初始化函数
            vector():start(0),finish(0),end_of_storage(0){};
            vector(size_type n,const value_type& x){
                fill_initialize(n,x);
            }
            vector(int n,const value_type& x){
                fill_initialize(n,x);
            }
            vector(long n,const value_type& x){
                fill_initialize(n,x);
            }

            explicit vector(size_type n){
                fill_initialze(n,T());
            }

            ~vector(){
                destroy(start,finish);
                deallocate();
            }

            reference front(){  return *begin(); }
            reference back() {  return *end()-1;  }

            void pop_back(){
                --finish;
                destory(finish);
            }

            iterator erase(iterator position){
                if(position+1!=finish){
                    copy(position+1,finish,position);
                }
                --finish;
                destory(finish);
                return position;
            }
            
            void resize(size_type new_size,const T& x){
                if(new_size<size())
                    erase(begin()+new_size,end());
                else
                    insert(end(),new_size-size(),x);
            }

            void resize(size_type new_size){ resize(new_size,T());}
            void clear(){erase(begin(),end());}

        protected:
        void insert_aux(iterator position, const T& x){
            if(finish!=end_of_storage) {
                construct(finish, *(finish - 1));
                finish++;
                T x_copy = x;
                copy_backward(position, finish - 2, finish - 1);    /////////////////
                *position = x_copy;
            }
            else {
                //原有空间已经用完
                const size_type old_size = size();
                const size_type len = old_size != 0 ? 2 * old_size : 1;
                iterator new_start = data_allocator::allocate(len);
                iterator new_finish = new_start;

                //try{
                new_finish = uninitialized_copy(start, position, new_start);
                construct(new_finish, x);
                ++new_finish;
                new_finish = uninitialized_copy(position, finish, new_finish);
                //}

                destroy(begin(),end());
                deallocate();

                start = new_start;
                finish = new_finish;

                end_of_storage = new_start + len;

            }
        }

        void deallocate(){
            if(start)
                data_allocator::deallocate(start, end_of_storage - start);
        }

        void fill_initialize(size_type n,const T& value){
            start=allocate_and_fill(n,value);
            finish =start+n;
            end_of_storage = finish;
        }
        //allocate_and_fill functino;start+n?=end;
            
        iterator allocate_and_fill(size_type n,const T& x){
                iterator result=data_allocator::allocate(n);
                unitialized_fill_n(result,n,x);
                return result;
        }
    };
}


//#include "Vector_imp.h"

#endif