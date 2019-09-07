
#ifndef _ALLOC_H_
#define _ALLOC_H_




#include <cstdlib>

//#include "



#if 0
#   include <new>
    #   define __THROW_BAD_ALLOC throw std::bad_alloc()
#elif !defined(__THROW_BAD_ALLOC)
#   include <iostream>
#   define __THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl;exit(1)
#endif


# define __NODE_ALLOCATOR_THREADS true


namespace tinySTL{


//一级配置器，用malloc and free  , __malloc_alloc_template
    template<int inst>
    class __malloc_alloc_template{

    private:
        //omm:out of memory
        static void *oom_malloc(size_t);
        static void *oom_realloc(void*, size_t);
        static void (* __malloc_alloc_oom_handler)();

    public:
        static void* allocate(size_t n){
            void* result = malloc(n);
            if(result==0)
                result = oom_malloc(n);
            return result;
        }

        static void* deallocate(void* p,size_t){
            free(p);
        }

        static void* reallocate(void* p, size_t /* old_sz */, size_t new_sz){
            void* result = realloc(p,new_sz);
            if(result ==0)
                result= oom_realloc(p,new_sz);
            return result;
        }

        static void (*set_malloc_handler(void (*f)()))(){
            void (* old)()=__malloc_alloc_oom_handler;
            __malloc_alloc_oom_handler= f;
            return old;
        }

    };


//malloc_alloc out of memory handling , 无法分配内存情况下的应对情况
    //初值为0，等客户端
    template<int inst>
    void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() =0;

    template<int inst>
    void* __malloc_alloc_template<inst>::oom_malloc(size_t n){
        void (* my_malloc_handler)();
        void* result;

        for(;;){
            my_malloc_handler = __malloc_alloc_oom_handler;
            if(my_malloc_handler ==0)//尝试释放，配置、再释放、再配置
                __THROW_BAD_ALLOC;
            (*my_malloc_handler)();
            result =malloc(n);
            if(result)
                return result;
        }
    }

    template<int inst>
    void* __malloc_alloc_template<inst>::oom_realloc(void* p,size_t n){

        void* result;
        void (*my_malloc_handler)();

        for(;;){
            my_malloc_handler=__malloc_alloc_oom_handler;
            if(my_malloc_handler==0)
                __THROW_BAD_ALLOC;
            (*my_malloc_handler)();
            result=realloc(p,n);
            if(result)
                return result;
        }
    }

    typedef __malloc_alloc_template<0>  malloc_alloc;  //把inst的初值设为0，别名malloc_alloc





//二级配置器 __default_alloc_template , 默认配置器，如果需求大于128则再调用一级配置器

    enum    {__ALIGN = 8};
    enum    {__MAX_BYTES = 128};
    enum    {__NFREELISTS =__MAX_BYTES/__ALIGN};

//二级配置器定义
    template<bool threads, int inst>
    class __default_alloc_template{

    private:
        static size_t ROUND_UP (size_t bytes){
            return ((bytes+__ALIGN-1) &~(__ALIGN-1));
        }
        //不知道怎么算的，设置成8的倍数

        union  obj {
            union   obj*    free_list_link;
            char    client_data[1]; //the client see this
        };

        static obj* volatile free_list[__NFREELISTS];

        static size_t FREELIST_INDEX(size_t bytes){
            return (((bytes)+__ALIGN-1)/__ALIGN-1);
        }

        static void* refill(size_t n);

        static char* chunk_alloc(size_t size,int &nobjs);

        //chunk allocation state
        static char*    start_free;//内存值起始位置
        static char*    end_free;
        static size_t   heap_size;

    public:
        static void* allocate(size_t n){
            obj* volatile* my_free_list;           ////////////
            obj* result;

            //大于128就调用第一级配置器
//核心:在这里决定要不要调用第一级配置器
            if(n> (size_t) __MAX_BYTES){
                return (malloc_alloc::allocate(n));
            }

            //寻找16个list中适当的一个
            my_free_list= free_list +FREELIST_INDEX(n);
            result = *my_free_list;
            if(result==0){
                void* r = refill(ROUND_UP(n));
                return r;
            }
            *my_free_list  = result->free_list_link;
            return (result);
        }



        static void  deallocate(void* p, size_t n){
            obj* q=(obj*)p;
            obj* volatile * my_free_list;

            if(n>((size_t)__MAX_BYTES)){
                malloc_alloc::deallocate(p,n);
            }

            //寻找list
            my_free_list = free_list + FREELIST_INDEX(n);

            //调整freelist，回收
            q->free_list_link = *my_free_list;
            *my_free_list = q;
        }
        static void* reallocate(void* p, size_t old_sz,size_t new_sz);
    };


    typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS,0> alloc;   //最常用的Alloc=alloc 中的alloc



    //重新填充freelist
    template<bool threads,int inst>
    void* __default_alloc_template<threads, inst>::refill(size_t n){
        int nobjs=20;
        //调用chuck_alloc
        // nojbs用的是passbyreference
        char* chunk = chunk_alloc(n,nobjs);
        obj*volatile* my_free_list;

        obj* result;
        obj* current_obj,*next_obj;
        int i;

        //如果只获得一个chunk，给调用者用，freelist无新节点
        if(nobjs==1)    return chunk;

        //否者重新调整freelist
        my_free_list = free_list +FREELIST_INDEX(n);

        //在chunk空间内建立freelist       //chunk空间？？？？
        result = (obj*) chunk;

        *my_free_list = next_obj = (obj*) (chunk+n);

        for(i=1;;i++){
            current_obj=next_obj;
            next_obj = (obj*) ((char*)next_obj + n);
            if(nobjs==i+1){
                current_obj->free_list_link=0;
                break;
            }else{
                current_obj->free_list_link = next_obj;
            }
        }
        return result;
    }

    //内存池

    template<bool threads, int inst>
    char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size,int& nobjs){
        char*   result;
        size_t  total_bytes= size*nobjs;
        size_t  bytes_left= end_free - start_free;

        if(bytes_left>=total_bytes){
            result = start_free;
            start_free += total_bytes;
            return result;
            //处理正常满足的情况
        }
        else if(bytes_left>=size){
            nobjs=bytes_left/size;
            total_bytes=size*nobjs;
            result=start_free;
            start_free+=total_bytes;
            return result;
        }
        else{
            size_t bytes_to_get = 2* total_bytes+ ROUND_UP(heap_size>>4);
            //试着利用 残余零
            if(bytes_left>0){
                //还有零头先给freelist
                obj* volatile* my_free_list = free_list+ FREELIST_INDEX(bytes_left);
                //调整freelist，将剩余的编入
                ((obj*)start_free)->free_list_link = *my_free_list;
                *my_free_list = (obj*)start_free;
            }

            //配置heap空间，补充内存值
            start_free = (char*)malloc(bytes_to_get);
            if(start_free==0){
                //malloc失败
                obj* volatile* my_free_list, *p;
                for(int i=size;i<=__MAX_BYTES;i+=__ALIGN){
                    my_free_list = free_list+ FREELIST_INDEX(i);
                    p = *my_free_list;
                    if(p!=0){
                        //freelist尚有未用区块，调整freelist释出未用区块
                        *my_free_list = p->free_list_link;
                        start_free= (char*)p;
                        end_free = start_free +i;
                        //递归调用自己，来修正nojbs
                        return chunk_alloc(size,nobjs);
                        //注意，任何残余零头终将被编入适当的freelist中备用
                        }
                }
                end_free=0;
                start_free = (char*)malloc_alloc::allocate(bytes_to_get);
            }
            heap_size+=bytes_to_get;
            end_free = start_free+bytes_to_get;
            return (chunk_alloc(size,nobjs));
        }
    }

    template <bool threads, int inst>
    char* __default_alloc_template<threads, inst>::start_free = 0;

    template <bool threads, int inst>
    char* __default_alloc_template<threads, inst>::end_free = 0;

    template <bool threads, int inst>
    size_t __default_alloc_template<threads, inst>::heap_size = 0;

    template <bool threads, int inst>
    typename __default_alloc_template<threads, inst>::obj* volatile
    __default_alloc_template<threads, inst>::free_list[__NFREELISTS]={
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    };






}


#endif