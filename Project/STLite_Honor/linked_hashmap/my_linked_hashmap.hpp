// Finished by MeteorVanish on 2022.5.4

// For reference only, please don't directly copy the code!!!
// If there is anything that can be improved, please feel free to let me know.
// Hope this code can help you solve the question!

#ifndef SJTU_LINKED_HASHMAP_HPP_STD
#define SJTU_LINKED_HASHMAP_HPP_STD

#include <cstddef>
#include <functional>
#include "utility.hpp"
#include "algorithm.hpp"
#include "exceptions.hpp"
#include "list.hpp"

const double LoadFactor = 0.75;
const size_t InitSize = 16;

namespace sjtu
{
    template <class Key, class Value, class Hash = std::hash<Key>, class Equal = std::equal_to<Key> >
    class linked_hashmap : public list<pair<const Key, Value> >
    {
    public:
        using value_type = pair<const Key, Value>;

    private:
        class Node : public list<value_type>::node
        {
        public:
            Node* after;
            /*
             * list<value_type>::node:
             *     value_type *val;
             *     node *prv, *nxt;
             */

            Node()
                    : list<value_type>::node(), after(nullptr) {}
            explicit Node(const value_type &_val, Node* _after = nullptr)
                    : list<value_type>::node(_val), after(_after) {}
            ~Node() = default;
        };

        class BucketList
        {
        public:
            Node* _head;

            BucketList() : _head(new Node) {}
            ~BucketList() {delete _head;}

            Node * find(const Key &o)
            {
                Node* p = _head->after;
                while(p != nullptr)
                {
                    if(Equal()(p->val->first, o)) break;
                    p = p->after;
                }
                return p;
            }
            Node * insert(const Key &k, const Value &v)
            {
                Node* add = new Node(value_type(k, v), _head->after);
                _head->after = add;
                return add;
            }
            Node * insert(const value_type &kv)
            {
                Node* add = new Node(kv, _head->after);
                _head->after = add;
                return add;
            }
            Node * erase(const Key &k)
            {
                Node *p = _head, *q;
                while(p != nullptr)
                {
                    q = p->after;
                    if(Equal()(q->val->first, k))
                    {
                        p->after = q->after;
                        return q;
                    }
                    p = q;
                }
                return nullptr;
            }
        };

        size_t Capacity;
        BucketList* storage;
        /*
         * list<pair<const Key, Value> >:
         *     size_t num;
         *     node *head, *tail, *nil;
         */

        void resize(size_t newCapacity)
        {
            Capacity = newCapacity;
            delete[] storage;
            storage = new BucketList[Capacity];
            for(typename list<value_type>::node* p = list<value_type>::head->nxt;
                p != list<value_type>::nil; p = p->nxt)
            {
                size_t _hash = Hash()(p->val->first) % Capacity;
                Node *q = dynamic_cast<Node*>(p), *hashHead = storage[_hash]._head;
                q->after = hashHead->after;
                hashHead->after = q;
            }
        }

    public:
        using iterator = typename list<value_type>::iterator;
        using const_iterator = typename list<value_type>::const_iterator;

        linked_hashmap() : list<value_type>(), Capacity(InitSize) { storage = new BucketList[Capacity]; }
        linked_hashmap(const linked_hashmap &other)
        {
            Capacity = other.Capacity;
            list<value_type>::num = 0;
            storage = new BucketList[Capacity];
            for(typename list<value_type>::node* p = other.head->nxt; p != other.nil; p = p->nxt)
            {
                size_t _hash = Hash()(p->val->first) % Capacity;
                Node* add = storage[_hash].insert(*p->val);
                list<value_type>::insert(list<value_type>::nil, add);
            }
        }
        linked_hashmap &operator=(const linked_hashmap &other)
        {
            if(this == &other) return *this;
            list<value_type>::clear();
            delete[] storage;
            Capacity = other.Capacity;
            list<value_type>::num = 0;
            storage = new BucketList[Capacity];
            for(typename list<value_type>::node* p = other.head->nxt; p != other.nil; p = p->nxt)
            {
                size_t _hash = Hash()(p->val->first) % Capacity;
                Node* add = storage[_hash].insert(*p->val);
                list<value_type>::insert(list<value_type>::nil, add);
            }
            return *this;
        }
        ~linked_hashmap() { delete[] storage; }

        Value &at(const Key &key)
        {
            size_t _hash = Hash()(key) % Capacity;
            Node* p = storage[_hash].find(key);
            if(p == nullptr) throw index_out_of_bound();
            return p->val->second;
        }
        const Value &at(const Key &key) const
        {
            size_t _hash = Hash()(key) % Capacity;
            Node* p = storage[_hash].find(key);
            if(p == nullptr) throw index_out_of_bound();
            return p->val->second;
        }
        Value &operator[](const Key &key)
        {
            size_t _hash = Hash()(key) % Capacity;
            Node* p = storage[_hash].find(key);
            if(p == nullptr)
                return insert(value_type(key, Value())).first->second;
            else
                return p->val->second;
        }
        const Value &operator[](const Key &key) const
        {
            size_t _hash = Hash()(key) % Capacity;
            Node* p = storage[_hash].find(key);
            if(p == nullptr) throw index_out_of_bound();
            return p->val->second;
        }
        void clear() override
        {
            list<value_type>::clear();
            delete[] storage;
            Capacity = InitSize;
            storage = new BucketList[Capacity];
        }
        pair<iterator, bool> insert(const value_type &value)
        {
            size_t _hash = Hash()(value.first) % Capacity;
            Node* p = storage[_hash].find(value.first);
            if(p != nullptr)
                return pair<iterator, bool>(iterator(p, this), false);
            p = storage[_hash].insert(value);
            list<value_type>::insert(list<value_type>::nil, p);
            if(list<value_type>::num > Capacity * LoadFactor) resize(Capacity << 1);
            return pair<iterator, bool>(iterator(p, this), true);
        }
        iterator erase(iterator pos) override
        {
            if(pos.invalid(this)) throw invalid_iterator();
            size_t _hash = Hash()((*pos).first) % Capacity;
            Node* p = storage[_hash].erase(pos->first);
            if(p == nullptr) throw invalid_iterator();
            list<value_type>::erase(pos);
            if(Capacity >> 1 >= InitSize && list<value_type>::num < (Capacity >> 2) * LoadFactor)
                resize(Capacity >> 1);
            return this->begin();
        }
        size_t count(const Key &key) const
        {
            size_t _hash = Hash()(key) % Capacity;
            return storage[_hash].find(key) != nullptr ? 1 : 0;
        }
        iterator find(const Key &key)
        {
            size_t _hash = Hash()(key) % Capacity;
            Node* p = storage[_hash].find(key);
            if(p != nullptr) return iterator(p, this);
            else return list<value_type>::end();
        }
        const_iterator find(const Key &key) const
        {
            size_t _hash = Hash()(key) % Capacity;
            Node* p = storage[_hash].find(key);
            if(p != nullptr) return const_iterator(p, this);
            else return list<value_type>::cend();
        }
    };
}

#endif