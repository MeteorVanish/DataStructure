// Finished by MeteorVanish on 2022.4.9

// For reference only, please don't directly copy the code!!!
// If there is anything that can be improved, please feel free to let me know.
// Hope this code can help you solve the question!

#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu
{
    template<typename T, class Compare = std::less<T>>
    class priority_queue
    {
    private:
        struct node
        {
            size_t height;
            T data;
            node *child, *sibling;
            node(size_t _height, T _data, node *_child = nullptr, node *_sibling = nullptr)
                    :height(_height), data(_data), child(_child), sibling(_sibling) {}
            ~node() = default;
        };

        size_t nowHeapNum, maxHeapNum;
        node **head;

        void DoubleSpace()
        {
            node **temp = head;
            delete[] head;
            maxHeapNum *= 2;
            head = new node *[maxHeapNum];
            for(size_t i = 1; i <= nowHeapNum; ++i)
                head[i] = temp[i];
        }
        node *Union(node *a, node *b)
        {
            if(!Compare() (a->data, b->data))
                std::swap(a, b);
            a->sibling = b->child;
            b->child = a;
            ++b->height;
            return b;
        }
        void MergeSubHeap(node *x)
        {
            size_t h = x->height;
            if(h > nowHeapNum)
            {
                nowHeapNum = h;
                head[h] = x;
                return;
            }
            bool flag = false;
            for(size_t i = h; i <= nowHeapNum; ++i)
                if(head[i])
                {
                    x = Union(head[i], x);
                    head[i] = nullptr;
                }
                else
                {
                    head[i] = x;
                    flag = true;
                    break;
                }
            if(!flag)
            {
                if(nowHeapNum == maxHeapNum - 1) DoubleSpace();
                head[++nowHeapNum] = x;
            }
        }
        void CopySubHeap(node *&pos, node *other)
        {
            if(other == nullptr) {pos = nullptr; return;}
            pos = new node(other->height, other->data);
            CopySubHeap(pos->child, other->child);
            CopySubHeap(pos->sibling, other->sibling);
        }
        void ClearSubHeap(node *pos)
        {
            if(pos == nullptr) return;
            ClearSubHeap(pos->child);
            ClearSubHeap(pos->sibling);
            delete pos;
        }
        void ClearAllSubHeap()
        {
            for(size_t i = 1; i <= nowHeapNum; ++i)
                if(head[i]) ClearSubHeap(head[i]);
            nowHeapNum = 0;
        }
        void pop_AddSubHeap(node *pos)
        {
            if(pos == nullptr) return;
            if(pos->height > 1)
                pop_AddSubHeap(pos->sibling);
            pos->sibling = nullptr;
            MergeSubHeap(pos);
        }
        size_t FindTopSubHeap() const
        {
            size_t pos;
            for(pos = 1; pos <= nowHeapNum; ++pos)
                if(head[pos]) break;
            for(size_t i = pos + 1; i <= nowHeapNum; ++i)
            {
                if(head[i] == nullptr) continue;
                if(!Compare() (head[i]->data, head[pos]->data))
                    pos = i;
            }
            return pos;
        }

    public:
        priority_queue() :nowHeapNum(0), maxHeapNum(100)
        {
            head = new node *[maxHeapNum];
        }
        priority_queue(const priority_queue &other) :nowHeapNum(other.nowHeapNum), maxHeapNum(other.maxHeapNum)
        {
            head = new node *[maxHeapNum];
            for(size_t i = 1; i <= nowHeapNum; ++i)
                CopySubHeap(head[i], other.head[i]);
        }
        ~priority_queue()
        {
            ClearAllSubHeap();
            delete[] head;
        }

        priority_queue &operator=(const priority_queue &other)
        {
            if(this != &other)
            {
                ClearAllSubHeap();
                delete[] head;
                nowHeapNum = other.nowHeapNum;
                maxHeapNum = other.maxHeapNum;
                head = new node *[maxHeapNum];
                for(size_t i = 1; i <= nowHeapNum; ++i)
                    CopySubHeap(head[i], other.head[i]);
            }
            return *this;
        }
        const T & top() const
        {
            if(empty())
                throw container_is_empty();
            size_t pos = FindTopSubHeap();
            return head[pos]->data;
        }
        void push(const T &e)
        {
            node *add = new node(1, e);
            MergeSubHeap(add);
        }
        void pop()
        {
            if(empty())
                throw container_is_empty();
            size_t pos = FindTopSubHeap();
            node *temp = head[pos]->child;
            delete head[pos], head[pos] = nullptr;
            pop_AddSubHeap(temp);
            if(head[nowHeapNum] == nullptr) --nowHeapNum;
        }
        size_t size() const
        {
            size_t cnt = 0;
            for(size_t i = 1; i <= nowHeapNum; ++i)
                if(head[i]) cnt += 1 << i - 1;
            return cnt;
        }
        bool empty() const {return !nowHeapNum;}
        void merge(priority_queue &other)
        {
            if(other.empty()) return;
            node *temp;
            for(size_t i = 1; i <= other.nowHeapNum; ++i)
                if(other.head[i])
                {
                    CopySubHeap(temp, other.head[i]);
                    MergeSubHeap(temp);
                }
            other.ClearAllSubHeap();
        }
    };

}

#endif