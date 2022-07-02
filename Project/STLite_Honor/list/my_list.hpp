// Finished by MeteorVanish on 2022.3.27

// For reference only, please don't directly copy the code!!!
// If there is anything that can be improved, please feel free to let me know.
// Hope this code can help you solve the question!

#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu
{
    template<typename T>
    class list
    {
    private:
        class node
        {
        public:
            T *data;
            node *prev, *next;

            explicit node(node *_prev = nullptr, node *_next = nullptr)
                    :data(nullptr), prev(_prev), next(_next) {};
            explicit node(const T &_data, node *_prev = nullptr, node *_next = nullptr)
                    :data(new T(_data)), prev(_prev), next(_next) {};
            ~node() {if(data) delete data;}
        };

    protected:
        node *head, *tail;
        size_t len;

        node *insert(node *pos, node *cur)
        {
            if(pos == head)
                throw runtime_error();
            cur->prev = pos->prev;
            cur->next = pos;
            cur->prev->next = cur;
            cur->next->prev = cur;
            ++len;
            return cur;
        }
        node *erase(node *pos)
        {
            if(pos == head || pos == tail)
                throw runtime_error();
            pos->prev->next = pos->next;
            pos->next->prev = pos->prev;
            --len;
            return pos;
        }

    public:
        class const_iterator;
        class iterator
        {
        private:
            const list<T> *plist;
            node *pos;

        public:
            explicit iterator(const list<T> *_plist = nullptr, node *_pos = nullptr)
                    :plist(_plist), pos(_pos) {}
            explicit iterator(const const_iterator &other)
                    :plist(other.getplist()), pos(other.getpos()) {}
            iterator(const iterator &other) = default;
            ~iterator() = default;

            const list<T> *getplist() const {return plist;}
            node *getpos() const {return pos;}

            iterator operator++(int)
            {
                if(pos->next == nullptr)
                    throw invalid_iterator();
                iterator temp = *this;
                pos = pos->next;
                return temp;
            }
            iterator & operator++()
            {
                if(pos->next == nullptr)
                    throw invalid_iterator();
                pos = pos->next;
                return *this;
            }
            iterator operator--(int)
            {
                if(pos->prev->prev == nullptr)
                    throw invalid_iterator();
                iterator temp = *this;
                pos = pos->prev;
                return temp;
            }
            iterator & operator--()
            {
                if(pos->prev->prev == nullptr)
                    throw invalid_iterator();
                pos = pos->prev;
                return *this;
            }
            T & operator *() const
            {
                if(pos == nullptr || pos->prev == nullptr || pos->next == nullptr)
                    throw invalid_iterator();
                return *(pos->data);
            }
            T * operator ->() const noexcept
            {
                if(pos == nullptr || pos->prev == nullptr || pos->next == nullptr)
                    throw invalid_iterator();
                return pos->data;
            }

            bool operator==(const iterator &rhs) const {return plist == rhs.getplist() && pos == rhs.getpos();}
            bool operator==(const const_iterator &rhs) const {return plist == rhs.getplist() && pos == rhs.getpos();}
            bool operator!=(const iterator &rhs) const {return !(*this == rhs);}
            bool operator!=(const const_iterator &rhs) const {return !(*this == rhs);}
        };

        class const_iterator
        {
        private:
            const list<T> *plist;
            const node *pos;

        public:
            explicit const_iterator(const list<T> *_plist = nullptr, node *_pos = nullptr)
                    :plist(_plist), pos(_pos) {}
            explicit const_iterator(const iterator &other)
                    :plist(other.getplist()), pos(other.getpos()) {}
            const_iterator(const const_iterator &other) = default;
            ~const_iterator() = default;

            const list<T> *getplist() const {return plist;}
            const node *getpos() const {return pos;}

            const_iterator operator++(int)
            {
                if(pos->next == nullptr)
                    throw invalid_iterator();
                const_iterator temp = *this;
                pos = pos->next;
                return temp;
            }
            const_iterator & operator++()
            {
                if(pos->next == nullptr)
                    throw invalid_iterator();
                pos = pos->next;
                return *this;
            }
            const_iterator operator--(int)
            {
                if(pos->prev->prev == nullptr)
                    throw invalid_iterator();
                const_iterator temp = *this;
                pos = pos->prev;
                return temp;
            }
            const_iterator & operator--()
            {
                if(pos->prev->prev == nullptr)
                    throw invalid_iterator();
                pos = pos->prev;
                return *this;
            }
            T & operator *() const
            {
                if(pos == nullptr || pos->prev == nullptr || pos->next == nullptr)
                    throw invalid_iterator();
                return *pos->data;
            }
            T * operator ->() const
            {
                if(pos == nullptr || pos->prev == nullptr || pos->next == nullptr)
                    throw invalid_iterator();
                return pos->data;
            }

            bool operator==(const iterator &rhs) const {return plist == rhs.getplist() && pos == rhs.getpos();}
            bool operator==(const const_iterator &rhs) const {return plist == rhs.getplist() && pos == rhs.getpos();}
            bool operator!=(const iterator &rhs) const {return !(*this == rhs);}
            bool operator!=(const const_iterator &rhs) const {return !(*this == rhs);}
        };

        list() :head(new node), tail(new node), len(0)
        {
            head->prev = nullptr;
            head->next = tail;
            tail->prev = head;
            tail->next = nullptr;
        }
        list(const list &other) :head(new node), tail(new node), len(other.len)
        {
            head->prev = nullptr;
            head->next = tail;
            tail->prev = head;
            tail->next = nullptr;
            for(node *i = other.head->next; i != other.tail; i = i->next)
            {
                node *add = new node(*i->data, tail->prev, tail);
                tail->prev->next = add;
                tail->prev = add;
            }
        }
        virtual ~list()
        {
            node *p = head, *q = head;
            while(q != nullptr)
            {
                q = p->next;
                delete p;
                p = q;
            }
        }
        list &operator=(const list &other)
        {
            if(&other != this)
            {
                clear();
                len = other.len;
                for(node *i = other.head->next; i != other.tail; i = i->next)
                {
                    node *add = new node(*i->data, tail->prev, tail);
                    tail->prev->next = add;
                    tail->prev = add;
                }
            }
            return *this;
        }
        const T & front() const
        {
            if(!len)
                throw container_is_empty();
            return *head->next->data;
        }
        const T & back() const
        {
            if(!len)
                throw container_is_empty();
            return *tail->prev->data;
        }
        iterator begin() {return iterator(this, head->next);}
        const_iterator cbegin() const {return const_iterator(this, head->next);}
        iterator end() {return iterator(this, tail);}
        const_iterator cend() const {return const_iterator(this, tail);}

        virtual bool empty() const {return !len;}
        virtual size_t size() const {return len;}

        virtual void clear()
        {
            len = 0;
            node *p = head->next, *q;
            while(p != tail)
            {
                q = p;
                p = p->next;
                delete q;
            }
            head->next = tail;
            tail->prev = head;
        }
        virtual iterator insert(iterator pos, const T &value)
        {
            if(pos.getplist() == nullptr || pos.getplist() != this)
                throw invalid_iterator();
            node *npos = pos.getpos();
            if(npos == head)
                throw invalid_iterator();

            ++len;
            node *add = new node(value, npos->prev, npos);
            add->prev->next = add;
            add->next->prev = add;
            return iterator(this, add);
        }
        virtual iterator erase(iterator pos)
        {
            if(pos.getplist() == nullptr || pos.getplist() != this)
                throw invalid_iterator();
            if(!len)
                throw container_is_empty();
            node *npos = pos.getpos();
            if(npos == head || npos == tail)
                throw invalid_iterator();

            --len;
            npos->next->prev = npos->prev;
            npos->prev->next = npos->next;
            iterator ret = iterator(this, npos->next);
            delete npos;
            return ret;
        }
        void push_back(const T &value)
        {
            ++len;
            node *add = new node(value, tail->prev, tail);
            tail->prev->next = add;
            tail->prev = add;
        }
        void pop_back()
        {
            if(!len)
                throw container_is_empty();
            --len;
            node *del = tail->prev;
            del->prev->next = del->next;
            del->next->prev = del->prev;
            delete del;
        }
        void push_front(const T &value)
        {
            ++len;
            node *add = new node(value, head, head->next);
            head->next->prev = add;
            head->next = add;
        }
        void pop_front()
        {
            if(!len)
                throw container_is_empty();
            --len;
            node *del = head->next;
            del->prev->next = del->next;
            del->next->prev = del->prev;
            delete del;
        }
        void sort()
        {
            if(!len) return;
            T *temp = (T *)malloc(len * sizeof(T));
            node *p = head->next;
            for(int i = 0; i < len; p = p->next, ++i)
            {
                new(temp + i) T(*p->data);
                p->data->~T();
            }
            sjtu::sort<T>(temp, temp + len, [](const T &a, const T &b) {return a < b;});
            p = head->next;
            for(int i = 0; i < len; p = p->next, ++i)
            {
                new(p->data) T(temp[i]);
                temp[i].~T();
            }
            free(temp);
        }
        void merge(list &other)
        {
            if(!other.len) return;
            for(node *p = head->next, *q = other.head->next; q != other.tail; )
            {
                if(p == tail || *q->data < *p->data)
                {
                    node *temp = other.erase(q);
                    q = temp->next;
                    insert(p, temp);
                }
                else p = p->next;
            }
        }
        void reverse()
        {
            if(!len) return;
            for(node *p = head; p != nullptr; p = p->prev)
            {
                node* temp = p->next;
                p->next = p->prev;
                p->prev = temp;
            }
            node *temp = head;
            head = tail;
            tail = temp;
        }
        void unique()
        {
            if(!len) return;
            T rec = *head->next->data;
            for(node *p = head->next->next; p != tail; )
            {
                if(*p->data == rec)
                {
                    node *del = erase(p);
                    p = p->next;
                    delete del;
                }
                else
                {
                    rec = *p->data;
                    p = p->next;
                }
            }
        }
    };
}

#endif //SJTU_LIST_HPP