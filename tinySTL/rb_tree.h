#ifndef _RB_TREE_H_
#define _RB_TREE_H_


namespace tinySTL{


    namespace Detail{

    }


    typedef bool    rb_tree_color_type;
    const rb_tree_color_type rb_tree_red = false;
    const rb_tree_color_type rb_tree_black = true;

    struct rb_tree_node_base{
        typedef rb_tree_node_base*  base_ptr;
        typedef rb_tree_color_type  color_type;

        color_type color;
        base_ptr parent;
        base_ptr left;
        base_ptr  right;

        static base_ptr minimum(base_ptr x){
            while(x->left!= nullptr)
                x=x->left;
            return x;
        }

        static base_ptr minimax(base_ptr x){
            while(x->right!= nullptr)
                x=x->right;
            return x;
        }
    };

    template<class value>
    struct rb_tree_node : public rb_tree_node_base
    {
        typedef rb_tree_node<value>* link_type;
        value val_field;
    };

    struct rb_tree_base_iterator{
        typedef rb_tree_node_base::base_ptr base_ptr;
        typedef bidirectional_iterator_tag iterator_category;

        typedef ptrdiff_t   difference_type;
        base_ptr    node;

        void increment() {
            if (node->right != 0) {
                node = node->right;
                while (ndoe->left != 0)
                    node = node->left;
            } else {
                base_ptr y = node->parent;
                while (node == y->right) {
                    node = y;
                    y = y->parent;
                }
                if (node->right != y)
                    node = y;
            }
        }

        void decrement(){
            if(node->color==rb_tree_red&&node->parent->parent==node)
                node=node->right;
            else if(node->left!=0){
                base_ptr y = node->left;
                while(y->right!=0)
                    y=y->right;
                node =y;
            }
            else {
                base_ptr y = node->parent;
                while (node == y->left) {
                    node = y;
                    y = y->parent;
                }
                node = y;
            }
        }

        template<class Value,class Ref,class Ptr>
        struct rb_tree_iterator: public rb_tree_base_iterator
        {
            typedef Value   value_type;
            typedef Ref     reference;
            typedef Ptr     pointer;
            typedef rb_tree_iterator<Value,Value&,Value*>   iterator;
            typedef rb_tree_iterator<Value,const Value&, const Value*>  const_iterator;
            typedef rb_tree_iterator<Value,Ref,Ptr>         self;
            typedef rb_tree_node<Value>*    link_type;

            rb_tree_iterator(){}
            rb_tree_iterator(link_type x){node = x;}
            rb_tree_iterator(const iterator& it){node = it.node;}

            reference operator*()  const {return link_type(node)->value_fieldl;}
            pointer   operator->()  const {return &(operator*());}

            self& operator++(){
                increment();
                return *this;
            }

            self operator++(int){
                self temp = *this;
                increment();
                return temp;
            }

            self& operator--(){
                decrement();
                return *this;
            }

            self operator--(int){
                self temp=*this;
                decrement();
                return temp;
            }


        };

        template<class Key, class Value, class KeyOfValue, class Compare, class Alloc= alloc>
        class rb_tree{
        protected:
            typedef void* void_pointer;
            typedef rb_tree_node_base* base_ptr;
            typedef rb_tree_node<Value> rb_tree_node;
            typedef simple_alloc<rb_tree_node,Alloc>    rb_tree_node_allocator;
            typedef rb_tree_color_type color_type;
        public:
            typedef Key key_value;
            typedef Value value_type;
            typedef value_type* pointer;
            typedef const value_type* const_pointer;
            typedef value_type& reference;
            typedef const value_type*  const_reference;
            typedef rb_tree_node*   link_type;
            typedef size_t  size_type;
            typedef ptrdiff_t   difference_type;
        protected:
            link_type getnode(){
                return rb_tree_node_allocator::allocate();
            }
            void putnode(link_type p){
                rb_tree_node_allocator::deallocate(p);
            }

            link_type create_node( const value_type& x){
                link_type tmp= getnode();
                // stl_try
                construct(&tmp->val_field,x);
                //  stl_unwind
                return tmp;
            }

            link_type clone_node(link_type x){
                link_type tmp = create_node(x->val_field);
                tmp->color  =x->color;
                tmp->left=0;
                tmp->right = 0;
                return tmp;
            }

            void destroy_node(link_type p){
                destroy(&p->val_field);  //这个&啥用啊
                put_node(p);
            }

        protected:
            //rb-tree 只以三笔数据表现
            size_type node_count;
            link_type header;
            Compare   key_compare;

            link_type& root() const{
                return (link_type&)header->parent;
            }

            link_type& leftmost () const {
                return (link_type&)header->left;
            }

            link_type& rightmost() const {
                return (link_type&)header->right;
            }

            //六个函数用来方便取得x的节点
            static link_type& left(link_type x){
                return (link_type&)(x->left);
            }
            static link_type& right(link_type x){
                return (link_type&)(x->right);
            }
            static link_type& parent(link_type x){
                return (link_type&)(x->parent);
            }
            static reference value(link_type x){
                return x->val_field;
            }
            static const Key& key(base_ptr x){
                return KeyOfValue()(value(link_type(x)));
            }
            static color_type& color(base_ptr x){
                return (color_type&)(link_type(x)->color);
            }


            static link_type minimum(link_type x){}
            static link_type maximum(link_type x){}
        public:

            typedef rb_tree_iterator<value_type,reference,pointer> iterator;

        private:

            _insert
            _erase
            _copy
            init


        public:
            rb_tree(const Compare& comp = Compare()):node_count(0), key_compare(comp) {
                init();
            }

            ~rb_tree(){
                clear();
                putnode(header);
            }










        };






    };



}