#ifndef _LIST_IMP_H_
#define _LIST_IMP_H_


#include<>
#
#


namespace tinySTL {
    namespace Detail {
        template<class T>
        list_iterator<T> &list_iterator<T>::operator++() {
            p = p->next;
            return *this;
        };

        template<class T>
        list_iterator<T> &list_iterator<T>::operator--() {
            p = p->prev;
            return *this;
        };

        template<class T>
        list_iterator<T> list_iterator<T>::operator++(int) {
            auto temp = *this;
            ++*this;
            return temp;
        };

        template<class T>
        list_iterator<T> list_iterator<T>::operator--(int) {
            auto temp = *this;
            --*this;
            return temp;
        };

        template<class T>
        bool list_iterator<T>::operator==(const list_iterator &lhs, const list_iterator &rhs) {
            return lhs.p == rhs.p;
        };

        template<class T>
        bool list_iterator<T>::operator!=(const list_iterator &lhs, const list_iterator &rhs) {
            return lhs.p != rhs.p;
        };
    }//end of Detial namespace


    //构造函数一类
    template<class T>
    list<T>::list() {
        head.p = nullptr;
        tail.p = head.p;
    }

    template<class T>
    list<T>::list(size_type n, const value_type &val = value_type()) {
        ctorAux(n, val, std::is_integral<value_type>());
    }

    template<class T>
    typename list<T>::nodePtr list<T>::newNode(cosnt value_type& val) {
        nodePtr res = nodeAllocator::allocate();
        nodeAllocator::construct(res, Detail::node<T>(val, nullptr, nullptr, this));
        return res;
    }

    template<class T>
    list<T>::list(const list &ls) {
        head.p = newNode();
        tail.p = head.p;
        for (auto node = ls.head.p; node != ls.tail.p; node = node->next)//这个->是正常定义么
            push_back(node->value);
    }

    template<class T>
    void list<T>::push_back(const value_type &val) {
        auto node = newNode();
        (tail.p)->value = val;
        (tail.p)->next = node;
        node->pre = tail.p;
        tail.p = node;
    }

    template<class T>
    template<class InputIterator>
    list<T>::list(InputIterator first, InputIterator last) {
        ctorAux(first, last, std::is_integral<value_type>());
    }

    template<class T>
    templte<class InputIterator>
    void list<T>::ctorAux(InputIterator first, InputIterator last, std::false_type) {
        head.p = newNode;
        tail.p = head.p;
        for (; first != last; ++first)
            push_back(*first);//这里push_back作用的类型，搞清楚
    }

    list &operator=(const list &ls);

    template<class T>
    list<T>::~list() {
        for (; head != tail;) {
            auto temp = head++;
            nodeAllocator::destory(temp.p);
            nodeAllocator::deallocate(temp.p);
        }
        nodeAllocator::deallocate(tail.p);
    }

    //基本属性，头尾操作
    template<class T>
    typename list<T>::size_type list<T>::size() const {
        size_type len = 0;
        for (auto i = head; i != tail; i++)
            len++;
        return len;
    }

    template<class T>
    void list<T>::push_front(const value_type &val) {
        auto temp = newNode(val);
        (head.p)->prev = temp;
        temp->next = head.p;
        head.p = temp;
    }

    template<class T>
    void list<T>::pop_back() {
        auto temp = (tail.p)->prev;
        temp->next = nullptr;
        deleteNode(tail.p);
        tail.p = temp;
    }

    template<class T>
    void list<T>::pop_front() {
        auto temp = head.p->next;
        temp->prev = nullptr;
        deleteNode(head.p);
        head.p = temp;
    }

    //迭代器相关
    template<class T>
    typename list<T>::iterator list<T>::begin() { return head; }

    template<class T>
    typename list<T>::iterator list<T>::end() { return tail; }

    template<class T>
    typename list<T>::cosnt_iterator begin() const {
        auto temp = (list * const)this;
        return changeIteratorToConstIterator(temp->head);
    }

    //辅助函数
    template<class T>
    typename list<T>::cosnt_iterator list<T>::changeIteratorToConstIterator(iterator &it) const {
        using nodeP = Detail::node<const T> *;
        auto temp = (list<const T> * const)this;
        auto ptr = it.p;
        Detail::node<const T> node(ptr->data, (nodeP) (ptr->prev), (nodeP) (ptr->next), temp);
        return cosnt_iterator(&node);
    }

    template<class T>
    typename list<T>::const_iterator list<T>::end() const {

    }

    //反转先放一边
    reverse_iterator rbegin();

    reverse_iterator rend();

    //相对高级算法
    template<class T>
    typename list<T>::iterator list<T>::insert(iterator position, const type_value &val) {
        if (position == begin()) {
            push_front(val);
            return begin();
        } else if (positio == end()) {
            auto ret = position;
            push_back(val);
            return ret;                     //有待考证，觉得有的不对
        }
        auto node = newNode(val);
        auto prev = position.p->prev;
        node->next = position.p;
        node->prev = prev;
        prev->next = node;
        position.p->prev = node;
        return iterator(node);
    }

    template<class T>
    void list<T>::insert(iterator position, size_type n, const value_type &val) {
        insert_aux(position, n, val, typename std::is_integral<InputIterator>::type());
    }

    template<class T>
    typename list<T>::iterator list::erase(iterator position) {
        if (position == head) {
            pop_front();
            return head;
        } else {
            auto prev = position.p->prev;
            prev->next = postion.p->next;
            position.p->next->prev = prev;
            deleteNode(position.p);
            return iterator(prev->next);
        }
    }

    template<class T>
    typename list<T>::iterator list<T>::erase(iterator first, iterator last) {

    }

    template<class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last)


    template<class T>
    void list<T>::remove(const value_type &val) {
        if (head.p->value == val) pop_front();
        if (tail.p->value == val) pop_back();
        auto cur = ++head;
        while (cur != tail) {
            if (cur.p->value == val) erase(cur);
        }
    }//有待验证目的和正确性


    //搞明白sort
    template<class T>
    void list<T>::sort() {
        sort(tinySTL::less<T>());
    }

    template<class T>
    void list<T>::deleteNode(nodePtr p) {
        p->prev = p->next = nullptr;
        nodeAllocator::destory(p);
        nodeAllocator::deallocate(p);
    }

    template<class Compare>
    void list<T>::sort(Compare comp) {


    }

    template<class T>
    void list<T>::reverse() {

    }

    template<class T>
    bool operator==(const list <T> &lhs, const list <T> &rhs) {
        auto node1 = lhs.head.p, node2 = rhs.head.p;
        for (; node1 != lhs.tail.p && node != rhs.tail.p; node1 = node->next, node2 = node2->next) {
            if (node1->data != node2->data)
                break;
        }
        if (node1 == lhs.tail.p && node2 == rhs.tail.p)
            return true;
        return false;
    }


}//end of tinySTL


#endif
