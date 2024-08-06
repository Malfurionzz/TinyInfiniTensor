#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "core/allocator.h"
namespace infini
{
    Allocator::Allocator(Runtime runtime) : runtime(runtime)
    {
        used = 0;
        peak = 0;
        ptr = nullptr;

        // 'alignment' defaults to sizeof(uint64_t), because it is the length of
        // the longest data type currently supported by the DataType field of
        // the tensor
        alignment = sizeof(uint64_t);

        freeBlockMap.insert({0,BLOCK_SZIE});

    }

    Allocator::~Allocator()
    {
        if (this->ptr != nullptr)
        {
            runtime->dealloc(this->ptr);
        }
    }

    size_t Allocator::alloc(size_t size)
    {
        IT_ASSERT(this->ptr == nullptr);
        // pad the size to the multiple of alignment
        size = this->getAlignedSize(size);
        for(auto it = freeBlockMap.begin(); it != freeBlockMap.end(); ++it){
            if( it->second >= size){
                auto addr = it->first;
                used += size;
                peak = std::max(peak,used);
                auto offset = it->first + size;
                auto freeSize = it->second - size;
                freeBlockMap.erase(it);
                freeBlockMap.insert({offset,freeSize});
                // std::printf("Allocte %ld bytes at %ld\n", size,addr);
                return addr;
                }
                throw std::runtime_error("No enough space in the block!");
            }
        // =================================== 作业 ===================================
        // TODO: 设计一个算法来分配内存，返回起始地址偏移量
        // =================================== 作业 ===================================

        return 0;
    }

    void Allocator::free(size_t addr, size_t size)
    {
        IT_ASSERT(this->ptr == nullptr);
        size = getAlignedSize(size);
        used -= size;
        auto it_m = freeBlockMap.insert({addr,size});
        auto it = it_m.first;
        auto it_n = std::next(it);
        if(it_n != freeBlockMap.end() && addr+size >= it_n->first){
            auto newSize = std::max(size,it_n->first + it_n->second - it->first);
            freeBlockMap.erase(it_n);
            it->second = newSize;
        }
        // std::printf("Free %ld bytes at %ld\n", size, addr);
        // =================================== 作业 ===================================
        // TODO: 设计一个算法来回收内存
        // =================================== 作业 ===================================
    }

    void *Allocator::getPtr()
    {
        if (this->ptr == nullptr)
        {
            this->ptr = runtime->alloc(this->peak);
            printf("Allocator really alloc: %p %lu bytes\n", this->ptr, peak);
        }
        return this->ptr;
    }

    size_t Allocator::getAlignedSize(size_t size)
    {
        return ((size - 1) / this->alignment + 1) * this->alignment;
    }

    void Allocator::info()
    {
        std::cout << "Used memory: " << this->used
                  << ", peak memory: " << this->peak << std::endl;
    }
}
