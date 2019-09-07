#ifndef _LIST_H_
#define _LIST_H_

#
#include "Iterator.h"
#include "Algorithm.h"
#include "Iterator.h"

#


#

namespace tinySTL{
    
    template<class T>
    //zhuyi
    class list; //这里是为啥

    namespace Detial{

        //定义一个list的node
        template<class T>
        struct node{
            T       value;
            node*   next;
            node*   prev;
            //为啥定义一个list<T>
            list<T> container;
            node(const T& d,node* n,node* p,list<T>* c):value(d),next(n),prev(p),container(c) {}
            //为啥来bool
            bool operator==(const node& n){
                return value==n.value&&next==n.next&&prev==n.prev&&container==n.container;
            }
        };
        
        //专为list的iterator
        template<class T>
        struct list_iterator:public iterator<bidirectional_iterator_tag,T>{
            template<class T>
            friend class list;
        public:
            typedef node<T>* nodePtr;
            nodePtr p;
        public:
            list_iterator(nodePtr ptr=nullptr):p(ptr){}
                
            list_iterator&  operator++();//前缀
            list_iterator   operator++(int);//后缀
            list_iterator&  operator--();
            list_irtrator   operator--(int);

            T& operator*()  {return p->value;}
            T* operator->() {return &(operator*());}

            bool operator==(const list_iterator<T>& lhs,const list_iterator<T>& rhs);
            bool operator!=(const list_iterator<T>& lhs,const list_iterator<T>& rhs);
            
        };
    }//end of Detial namespace
        

        //定义class list
        template<T>
        class list{
            template<class>
            friend struct list_iterator;
        private:
            typedef simple_alloc<Detail::node<T>> nodeAllocator;
            typedef Detail::node<T>* nodePtr;
        public:
            typedef T                                   value_typel;
            typedef size_t                              size_type;
            typedef Detail::list_iterator<T>            iterator;
            typedef Detail::list_iterator<const T>      const_iterator;
            typedef reverse_iterator_t<iterator>        reverse_iterator;
            typedef T&                                  reference;
        private:
            iterator head;
            iterator tail;
            //为啥不用list_iterator
        public:
            list();   //w
            explicit list(size_type n, const value_type& val = value_type()); //w
            template<class InputIterator>
            list(InputIterator first,InputIterator last);//w
            list(const list& l);//w
            list& operator=(const list& ls);
            ~list();//w

        public:
            bool empty(){return head==tail;}
            size_type size() const;
            reference front(){return head.p->value;}
            reference back(){return tail.p->value;}
            
            void push_back(const value_type& val);
            void push_front(const value_type& val);
            void pop_back();
            void pop_front();

            iterator begin();
            iterator end();
            const_iterator begin() const;
            const_iterator end()    const;
            reverse_iterator rbegin();
            reverse_iterator rend();

            iterator insert(iterator position, const type_value& val);
            iterator erase(iterator position);
            iterator erase(iterator first, iterator last);
            template<class InputIterator>
            void insert(iterator position, InputIterator first, InputIterator last)
            void remove(const value_type& val)
            void sort();
            template<class Compare>
            void sort(Compare comp);
            void reverse();

        private:
            template<class InputIterator>
            void ctorAux(InputIterator first,InputIterator last, ste::false_type);//
            void ctorAux();    //w
            nodePtr newNode(); //w
            void deleteNode
        
        public:
            template<class T>
            friend void swap(list<T>& lx,list<T>& ly);
            template<class T>
            friend bool operator==(const list<T>& lx,const list<T>& ly);
            template<class T>
            friend bool operator!=(const list<T>& lx,const list<T>& ly);
        };
}


#include "list_imp.h"
#endif
