#include "mem.c"
#include "string.h"
//当目的内存>=源内存大小时，直接复制
//当目的内存< 源内存大小时,把src中的内容填入到dest中，直到内存填满了
void mem_copy(void* dest,void* src){
    int dest_size= strlen(dest);
    int src_size = strlen(src);

    if(dest_size >= src_size){
        for(int i=0;i<src_size;i++)
            dest[i]=src[i];
        
    }
    else{
        for(int i=0;i<dest_size;i++)
            dest[i]=src[i];
    }
}

//复制的内存
void mem_copy(void* dest,void* src,size_t mem_size){

}