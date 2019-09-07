#ifndef _DEQUE_IMP_H_
#define _DEQUE_IMP_H_



namespace tinySTL{
    namespace Detail {

        inline size_t dq_buffer_size(sizt_t _size) {
            return _size < 512 ? size_t(512 / _size) : size_t(1);
        }

        template<class T, class Ref, class Ptr>
        struct dq_iter {

            typedef dq_iter<T, T &, T *> iterator;
            typedef dq_iter<T, const T &, const T *> const_iterator;

            static size_t buffer_size() { return dq_buffer_size(sizeof(T)); }

            typedef random_access_iterator_tag iterator_categroy;
            typedef T value_type;
            typedef Ptr pointer;
            typedef Ref reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
            typedef T **map_pointer;

            typedef dq_iter self;

            T *m_cur;
            T *m_first;
            T *m_last;
            map_pointer m_node;

            reference operator*() { reuturn *m_cur; }

            pointer operator->() { return m_cur; }

            void set_node(map_pointer new_node) {
                node = new_node;
                first = *new_node;
                last = first + difference_type(buffer_size());
            }

            difference_type operator-(const self &x) {
                return difference_type(buffer_size()) * (m_node - x.m_node - 1) + (cur - first) + (x.last - x.cur);
            }

            self &operator++() {
                ++cur;
                if (cur == last) {
                    set_node(node + 1);
                    cur = first;
                }
                return *this;
            }

            self operator++(int) {   //后置
                self temp = *this;
                ++*this;
                return temp;
            }

            self &operator--() {
                if (cur == first) {
                    set_node(m_node - 1);
                    cur = last;
                }
                --m_cur;
                return *this;
            }

            self operator--(int) { //后置
                self temp = *this;
                --*this;
                return temp;
            }

            self &operator+=(difference_type n) {
                difference_type offset = n + m_cur - m_first;
                if (offset >= 0 && offset < difference_type(buffer_size()))
                    m_cur += n;
                else {
                    difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size()) :
                                                  -difference_type((-offset - 1) / buffer_size()) - 1;
                    set_node(m_node + node_offset);
                    m_cur = m_first + (offset - node_offset * difference_type(buffer_size()));
                }
                return *this;
            }

            self operator+(difference_type n) const {
                self = temp = *this;
                return *this += n;
            }

            self operator-=(difference_type n) { return *this += -n; }

            reference operator[](difference_type n) const { return *(*this + n); }  //没看懂

            bool operator==(const self &x) const { return m_cur == x.m_cur; }

            bool operator<(const self &x) const {
                return (m_node == x.m_node) ? (m_cur < x.m_cur) : (m_node < x.m_node)
            }

            bool operator>(const self &x) const { return x < *this; };

            bool operator<=(const self &x) const { return !(x < *this); }


            dq_iter(T *x, map_pointer y) : m_cur(x), m_first(*y), m_last(*y + buffer_size()), mod_node(y) {}

            dq_iter() : m_cur(0), m_first(0), m_last(0), m_node(0) {}

            dq_iter(const iterator &x) : m_cur(x.m_cur), m_first(x.m_first), m_last(x.m_last), m_node(x.m_node) {}

        };

        template<class T, class Ref, class Ptr>
        inline dq_iter<T, Ref, Ptr> operator+(ptrdiff_n, const dq_iter<T, Ref, Ptr> &x) {
            return x + n;
        }//没懂
    }

#ifndef _bulabula_
        template<class T, class Ref, class Ptr>
        iterator_access_iterator_tag iterator_category(cosnt dq_iter<T,Ref,Ptr>& ) {return random_access_iterator_tag();}

#endif  //bulabula

        //deque base class for ordinary allocators
        //construct and destroy storage, make the exception safety easiyer
        //encapsaltus all the differencd between STD sTL and SGI STL
        /*
        template<class T, class Alloc, bool is_static>
        class dq_alloc_base{

        public:
            typedef typename alloc_traits<T, alloc>::allocator_type allocator_type;
            allocator_type get_allocator() const {return m_node_allocator}

        };
         */

        template <class T, class Alloc = alloc ,size_t BufSiz =0>
        class deque{
        public:
            typedef T                                   value_type;
            typedef T*                                  pointer;
            typedef T&                                  reference;  //为啥没哟
            typedef size_t                              size_type;

            typedef dq_iter<T,T*,T&,BufSiz>             iterator;

        protected:
            typedef pointer*                            map_pointer;
        protected:
            iterator    start;
            iterator    finish;
            map_pointer map;
            size_type   map_size;

        public:
            iterator begin(){ return start;}
            iterator end()  {return finish;}
            reference   operator[](size_type n){
                return start[difference_type(n)];    //用了 dq_iter 的 operator【】
            }
            reference  front(){ return *start;}
            reference  back(){
                iterator    temp = finish;
                temp--;
                return *temp;
            }

            size_type size() const { return finish - start;}
            size_type max_size

            bool empty() const { return start == finish ;}

        protected:
            //专属空间配置器
            typedef simple_alloc<value_type , Alloc>          data_allocator;
            typedef simple_alloc<pointer, Alloc>              map_allocator;

            deque(int n, const value_type& value):start(0),finish(0),map(0),map_size(0){
                fill_initialize(n,value);
            }

            template <class T, class Alloc, size_t BufSiz>
            void deque<T,Alloc,BufSiz>::fill_initialize(size_type n, const value_type& value){
                create_map_and_notes(n);
                map_pointer cur;
                __STL_TRY{
                        for(cur =start.node ;cur<finish.node;cur++){
                            unitialized_fill(*cur,*cur+dq_buffer_size(),value);
                            //最后一个节点有不同
                            uninitialized_fill(*finish.first,*finish.cur, value);
                        }

                }
                catch(){
                    ...
                }
            }

            template<class T, class Alloc, size_t BufSiz>
            void deque<T,Alloc,BufSiz>::creat_map_and_notes(size_type num_elements){
                size_type num_nodes=num_elements/buffer_size+1;
                map_size =max(initail_map_size(), num_nodes +2);
                map = map_alocator::allocate(map_size);

                map_pointer nstart = map+(map_size-num_nodes)/2;
                map_pointer nfinish =nstart + num_nodes-1;   //这俩没看懂没看懂没看懂

                map_pointer cur;
                __STL_TRY{
                        for(cur=nstart;cur<=nfinish;cur++){
                            *cur = allocate_node();
                        }

                }
                catch(..){
                    // commit or rollback :若不全部，就一个不留·
                }
                start.set_node(nstart);
                finish.set_node(nfinish);
                start.cur = start.first;
                finish.cur= finish.first+num_elements%buffer_size();

            }

        public:
            void push_back(const value_type& value){
                if(finish.cur!=finish.last-1) {
                    construct(finish.cur, value);
                    ++finish.cur;
                }
                else
                    push_back_aux(value);
            }

            void push_back_aux(const value_type& value);
            void deque<T,Alloc,BufSiz>::push_back_aux(const value_type& value){
                value_type t_copy=value;
                reserve_map_at_back();
                *(finish.m_node+1)= allocate_node();
                __STL_TRY{
                    construct(finish.cur,t_copy);
                    finish.set_node(finish.node+1);
                    finish.cur= finish.first;
                }

            }


            void push_front(const value_type& value){
                if(start.cur!=start.first) {
                    construct(start.first, value);
                    start.cur--;
                }
                else{
                    push_front_aux(value);
                }
            }

            void deque<T,Alloc,BufSiz>::push_front_aux(const value_type& value){
                value_type t_copy= value;
                reserve_map_at_front();
                *(start.m_node-1)= allocate_node();

                construct(start.last-1,t_copy);
                start.set_node(start.m_node-1);
                start.cur=start.last;
            }

            void reserve_map_at_back(size_type nodes_to_add=1){
                if(nodes_to_add+1>map_size-(finish.node-map))
                    reallocate_map(nodes_to_add,false);
            }

            void reallocate_map(size_type nodes_to_add, bool add_at_front){
                size_type old_num_nodes = finish.node-start.node+1;
                size_type new_num_nodes = old_num_nodes + nodes_to_add;

                map_pointer new_nstart;
                if(map_size >2*new_num_nodes){
                    new_start =map+(map_size-new_num_nodes)/2+
                            (add_at_front? nodes_to_add:0);
                    if(new_nstart<start.node)
                        copy(start.node,finish.node+1,new_start);
                    else
                        copy_backword(start.node,finish.node+1,new_nstart+old_num_nodes);
                }
                else{
                    size_type new_map_size=map_size + max(map_size,nodes_to_add)+2;
                    map_pointer  new_map = map_allocator::allocate(new_map_size);
                    new_nstart=new_map  + (new_map_size-new_num_nodes)/2+
                            (add_at_front?nodes_to_add:0);
                    copy(start.node,finish.node,new_nstart);
                    map_allocator ::deallocate(map,map_size);
                    map=new_nstart;
                    map_size= new_map_size;
                }
                start.set_node(new_nstart);
                finish.set_node(new_nstart+old_num_nodes-1); //为啥为啥
            }


            void pop_back();
            void pop_back_aux();
            void pop_front();
            void pop_front_aux();


            void deque<T,Alloc,BufSiz>::pop_back(){
                if(finish.cur==finish.first){
                    pop_back_aux();
                }
                else{
                    finish.cur--;
                    destroy(finish.cur);
                }
            }

            void deque<T,Alloc,BufSiz>::pop_back_aux(){
                deallocate_node(finish.cur);
                finish.set_node(finish.node-1);
                finish.cur=finish.last-1;
                destroy(finish.cur);
            }

            template<class T, class Alloc, size_t BufSiz>
            deque<T,Alloc,BufSiz>::iterator
            deque<T,Alloc,BufSiz>::clear(){
                for(map_pointer node = start.node+1;node<finish.node;node++) {
                    destroy(*node, *node + buffer_size());
                    data_allocator::deallocate(*node,*node+buffer_size());
                }
                if(start.node!=finish.node){
                    destroy(*start.node,*start.node+buffer_size());
                    destroy(*finish.node,*finish.node+buffer_size());
                    data_allocator ::deallocate(*finish.node,buffer_size());
                }
                else{
                    destroy(*start.node,*start.node+buffer_size());
                }
                finish=start;
            }

            iterator erase(iterator pos);

            template<class T, class Alloc, size_t BufSiz>
            deque<T,Alloc,BufSiz>::iterator
            deque<T,Alloc,BufSiz>::erase(iterator pos){
                iterator next = pos;
                ++next;
                difference_type index = pos-start;
                if(index<(size()>>1)){
                    copy_backward(start,pos,next);
                    pop_front();
                }
                else{
                    copy(next,finish,pos);//搬移清除点之后的元素
                    pop_back();
                }
                return start+index;
            }

            template<class T, class Alloc, size_t BufSiz>
            deque<T,Alloc,BufSiz>::iterator
            deque<T,Alloc,BufSiz>::erase(iterator first, iterator last) {
                if (first == start && last == finish) {
                    clear();
                    return finish;
                } else {
                    difference_type n = last - first;
                    difference_type element_bef = first - start;
                    if (element_bef < (size() - n) / 2) {
                        copy_backward(start, first, last);
                        iterator new_start = start + n;
                        destroy(start, new_start);
                        for (map_pointer cur = start.node; cur < new_start.node; cur++) {
                            data_allocator::deallocate(*cur, buffer_size());
                        }
                        start = new_start;
                    }
                }
                else{
                    copy(last, finish, first);
                    iterator new_finish = finish - n;
                    destroy(new_finish, finish);
                    for (map_pointer cur = new_finish.node + 1; cur < finish.node; cur++) {
                        data_allocator::deallocate(*cur, buffer_size())
                    }
                    finish = new_finish;

                }
                return start + element_bef;
            }


            template<class T, class Alloc, size_t BufSiz>
            deque<T,Alloc,BufSiz>::iterator
            deque<T,Alloc,BufSiz>::insert(iterator pos, const value_type& value) {
                if (pos.cur == start.cur) {
                    push_front(x);
                    return start;
                } else if (pos.cur == finish.cur) {
                    push_back(x);
                    iterator temp = finish;
                    temp--;
                    return temp;
                } else {
                    return insert_aux(pos, value);
                }
            }

            template<class T, class Alloc, size_t BufSiz>
            deque<T,Alloc,BufSiz>::iterator
            deque<T,Alloc,BufSiz>::insert_aux(iterator pos, const value_type& value) {
                difference_type index = pos-start;
                value_type value_copy= value;
                if(index<(size()/2)) {
                    push_front(front());
                    iterator front1 = start;
                    ++front1;
                    iterator front2 = front1;
                    ++front2;
                    pos = start + index;
                    iterator pos1 = pos;
                    ++pos1;
                    copy(front2, pos1, front1);
                }
                else {
                    push_back(back());
                    iterator back1 = finish;
                    --back1;
                    iterator back2 = back1;
                    --back2;
                    pos = start + index;
                    copy_backward(pos, back2, back1);
                }
                *pos= x_copy;
                return pos;
            }

        };
    }
}


