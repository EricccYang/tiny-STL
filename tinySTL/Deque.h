#ifndef _DEQUE_H_
#define _DEQUE_H_


#include "Deque_imp.h"
#include "Iterator.h"
#


namespace tinySTL{
    template<class T,class Alloc = allocator<T>>
    class deque;
    namespace Detail{

        template<class T>
        class deq_iter:public iterator<bidirectional_iterator_tag, T> {
        
        //作用是什么
        private:
            template<class T,class Alloc>
            friend class::tinySTL::deque;
        private:
            typedef cosnt ::tinySTL::deque<T>* cntrPtr;
            cntrPtr container;
            size_t  mapIndex;
            T*      cur;
        public:
            deq_iter():mapIndex(-1),cur(0),container(0){}
            deq_iter(size_t mindex,T* ptr,cntrPtr container_input):mapIndex(mindex),cur(ptr),container(container_input){}
            deq_iter(const deq_iter& itr):mapIndex(itr.mapIndex),cur(itr.cur),container(itr.container){}
            deq_iter& operator=(const deq_iter& it);
            void swap(deq_iter& it);
            reference   operator*(){ return *cur;}
            pointer     operator->(){ return &(operator*());}
            //为什么要有个const版本
            const reference   operator*() const  { return *cur;}
            const pointer     operator->() const { return &(operator*());}

            deq_iter&   operator++();
            deq_iter    operator++(int);
            deq_iter&   operator--();
            deq_iter    operator--(int);

            bool operator==(const deq_iter& it) const;
            bool operator!=(const deq_iter& it) const;
        private:
            pointer getBuckTail(size_t mapIndex) const;
            pointer gerBuckHead(size_t mapIndex) const;
            size_t getBuckSize() const;
        public:
            template<class T>
            friend deq_iter<T> operator+(const deq_iter<T>& it, typename deq_iter<T>::difference n);
            template<class T>
            friend deq_iter<T> operator+(typename deq_iter<T>::difference n, const deq_iter<T>& it);
            template<class T>
            friend deq_iter<T> operator-(const deq_iter<T>& it, typename deq_iter<T>::difference n);
            template<class T>
            friend deq_iter<T> operator-(typename deq_iter<T>::difference n, const deq_iter<T>& it);
            template<class T>
            friend typename deq_iter<T>::difference operator- (const deq_iter& it1, const deq_iter& it2);
            template<class T>
            friend void swap (deq_iter<T>& lhs, deq_iter<T>& rhs);
        };//end of class deq_iter
    }//end of Detail namespace
    
    
    //begin of define of deque container
    template<class T>
    class deque{
        public:
            typedef T                   value_type;
            typedef Detail::deq_iter<T> iterator;
            //注意一下这个
            typedef Detail::deq_iter<const T>   const_iterator;
            typedef T&                  reference;
            typedef size_t              size_type;
            typedef ptrdiff_t           difference_type;
            typedef Alloc               allocator_type;
            typedef const reference     const_reference;
        private:
            typedef Alloc dataAllocator;
            enum class EBuckSize{BUCKSIZE = 64};
        private:
            iterator first, last;
            size_t   mapSize;
            T**      map;
        public:
            deque();
            explicit deque(size_type n, const value_type& val = value_type());
            template<class InputIterator>
            deque(InputIterator first, InputIterator last);
            deque(const deque& x);

            ~deque();

            deque& operator=(const deque& x);
            deque& operator=(deque&& x);

            iterator begin();
            iterator end();
            iterator begin()const;
            iterator end()const;
    
        public:
            size_type size() const{ return end()-begin();}
            bool empty() const  { return begin()==end();}

            reference operator[] (size_type n);
            reference front();
            reference back();
            const_reference operator[] (size_t n) const;
            const_reference front()     const;
            const_referecne back()      const;

            void push_back(const value_type& val);
            void push_front(const value_type& val);
            void pop_back();
            void pop_front();
            void swap(deque& x);
            void clear();

        private:
            T* getANewBuck();
            T** getANewMap(const size_t size);
            size_t getNewMapSize(const size_t size);
            size_t getBuckSize() const;
            void init();
            bool back_full() const;
            bool front_full() const;
            void deque_aux(size_t n, value_type& val, std::true_type);
            template<class Iterator>
            void deque_aux(Iterator first, Iterator last, std::false_type);
            void reallocateAndCopy();
        public:
            template<class T, class Alloc>
            friend bool operator == (const deque<T,Alloc>& lhs, deque<T,Alloc>& rhs);
            template<class T, class Alloc>
            friend bool operator != (const deque<T,Alloc>& lhs, deque<T,Alloc>& rhs);
            template<class T, class Alloc>
            friend void swap(deque<T,Alloc>& x, deque<T,Alloc>& y);
    };//end of class deque
}
            

















