//practice for iterator part

//原生auto_ptr

template <class T>
class auto_ptr{
public:
    explicit auto_ptr(T* p = 0):pointee(p) {}
    template<class U>
    auto_ptr(auto_ptr<U>& rhs):pointee(rhs.release()){}
    ~auto_ptr() {delete pointee;}

    template<class U>
    auto_ptr<T>& operator=(auto_ptr<U>& rhs){
        if(this !=&rhs) reset(rhs.release());
        return *this;
    }

    T& operator*() const {return *pointee}
    T* operator->() const (return pointee}
    T* get() const {return pointee}
    //..

//例子，看list的find怎么实现
private:
    T* pointee;
};


//list.h
template<class T>
class List{
private:
    ListItem<T>* _front;
    ListItem<T>* _end;
    long _size;
public:
    void insert_front(T value);
    void insert_end(T value);
    void display(std::ostream &os = std::cout) const;
    //...
};

template<class T>
class ListItem{
private:
    ListItem* _next;
    T value;
public:
    void 

};


//list_iter.h
#include "list.h"
template<class Item>
struct ListIter{
    Item* ptr;
    
    ListIter(Item* p = 0):ptr(p) {}

    //不必实现copy 和 operator= 因为编译器提供的缺省已足
    Item&   operator*() const {return *ptr}
    Item*   operatro->()    const {return ptr}

    //
    ListItem& operator++() {ptr=ptr->next; retrun *this}
    ListItem* operator++(int)  {ListIter temp =*this;++*this;return temp;}

    bool operator==(const ListIter& i) {return ptr==i.ptr;}
    bool operator!=(const ListIter& i) {return ptr!=i.ptr;}
};




