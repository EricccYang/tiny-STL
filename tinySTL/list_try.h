#ifndef _LIST_H
#defien _LIST_H
//还没有说namespace

template<class T>    
struct __list_node{
    
    T data;
    __list_node<T>* prev;
    __list_node<T>* next;
};

//list的迭代器

template<class T, class Ref, class Ptr>
class __list_iterator{
    
    typedef __list_iterator<T,T&,T*>    iterator;
    typedef __list_iterator<T,Ref,Ptr>  self;

    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __listnode<T>* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t   difference_type;
    

    //成员
    link_type node;

    //constructor
    __list_iterator(link_type x):node(x){}
    __list_iterator(){}
    __list_iterator(const iterator& x):node(x.node){}

    //重载运算符
    bool operator==(self& x)const
    {return node==x.node;}
    bool operator!=(self& x)const   {return node!=x.node;}
    
    //dereference
    reference operator* () const    {return (*node).data;}

    pointer operator->() const  {return &(operator*());}
    
    self& operator++(){
        node= (link_type)((*node).next);
        return *this;
    }
    self& operator--(){
        node = (link_type)((*node).prev);
        return *this;
    }

    self operator++(int){
        self tmp = *this;
        ++this;
        return tmp;
    }

    self operator--(int){
        self tmp = *this;
        --this;
        return tmp;
    }

};



//list的容器设计
template<class T, class Allco= alloc>
class list{
protected:
    typedef __list_node<T> list_node;

public:
    typedef listnode* link_type;

protected:
    typedef link_type node;
};

iterator begin() {return (link_type)((*node).next);}
iterator end()  {return node;}

bool empty() const{return node->next == node;}
size_type size() const {
    size_type result =0;
    distance(begin(),end(),result);
    return result;
}

reference front() {
    return *begin();
}
reference back(){
    return *(--end());
}


