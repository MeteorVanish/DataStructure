// Finished by MeteorVanish on 2022.6.1

// For reference only, please don't directly copy the code!!!
// If there is anything that can be improved, please feel free to let me know.
// Hope this code can help you solve the question!

// Implemented with AVL tree

#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu
{
    template<class Key, class T, class Compare = std::less<Key> >
    class map
    {
    public:
        typedef pair<const Key, T> value_type;

    private:
        bool Equal(const Key &x, const Key &y) const { return !(Compare()(x, y) || Compare()(y, x)); }

        struct Node
        {
            value_type *data;
            size_t height;
            Node *Lchild, *Rchild;

            Node() :data(nullptr), height(0), Lchild(nullptr), Rchild(nullptr) {}
            Node(const value_type &_data)
                    :data(new value_type(_data)), height(1), Lchild(nullptr), Rchild(nullptr) {}
            ~Node() { delete data; }
        };

        Node* root;
        Node* nil;
        size_t Size;

        size_t GetHeight(Node *t) { return t ? t->height : 0; }

        void AdjustHeight(Node *t) {t->height = std::max(GetHeight(t->Lchild), GetHeight(t->Rchild)) + 1; }

        void LL(Node *&t)
        {
            Node* tLC = t->Lchild;
            t->Lchild = tLC->Rchild;
            tLC->Rchild = t;
            AdjustHeight(t);
            AdjustHeight(tLC);
            t = tLC;
        }

        void RR(Node *&t)
        {
            Node* tRC = t->Rchild;
            t->Rchild = tRC->Lchild;
            tRC->Lchild = t;
            AdjustHeight(t);
            AdjustHeight(tRC);
            t = tRC;
        }

        void LR(Node *&t)
        {
            RR(t->Lchild);
            LL(t);
        }

        void RL(Node *&t)
        {
            LL(t->Rchild);
            RR(t);
        }

        void Clear(Node *&t)
        {
            if(t == nullptr) return;
            Clear(t->Lchild);
            Clear(t->Rchild);
            delete t;
            t = nullptr;
        }

        Node* Find(const Key &target, Node *t) const
        {
            if(t == nullptr) return nil;
            const Key &cur = t->data->first;
            if(Equal(cur, target)) return t;
            if(Compare()(target, cur)) return Find(target, t->Lchild);
            else return Find(target, t->Rchild);
        }
        Node* Find(const Key &target) const { return Find(target, root); }

        Node* Insert(const value_type &val, Node* &t)
        {
            Node* ret;
            if(t == nullptr)
            {
                t = new Node(val);
                return t;
            }
            if(Equal(val.first, t->data->first)) return t;
            if(Compare()(val.first, t->data->first))
            {
                ret = Insert(val, t->Lchild);
                if(GetHeight(t->Lchild) - GetHeight(t->Rchild) == 2)
                {
                    if(Compare()(val.first, t->Lchild->data->first)) LL(t);
                    else LR(t);
                }
            }
            else
            {
                ret = Insert(val, t->Rchild);
                if(GetHeight(t->Rchild) - GetHeight(t->Lchild) == 2)
                {
                    if(Compare()(t->Rchild->data->first, val.first)) RR(t);
                    else RL(t);
                }
            }
            AdjustHeight(t);
            return ret;
        }

        bool Adjust(Node *&t, size_t dir)
        {
            if(dir == 0) //LeftSubTree
            {
                if(GetHeight(t->Rchild) - GetHeight(t->Lchild) == 1) return true;
                if(GetHeight(t->Lchild) == GetHeight(t->Rchild)) { --t->height; return false; }
                if(GetHeight(t->Rchild->Lchild) > GetHeight(t->Rchild->Rchild)) { RL(t); return false; }
                if(GetHeight(t->Rchild->Lchild) < GetHeight(t->Rchild->Rchild)) { RR(t); return false; }
                if(GetHeight(t->Rchild->Lchild) == GetHeight(t->Rchild->Rchild)) { RR(t); return true; }
            }
            else         //RightSubTree
            {
                if(GetHeight(t->Lchild) - GetHeight(t->Rchild) == 1) return true;
                if(GetHeight(t->Lchild) == GetHeight(t->Rchild)) { --t->height; return false; }
                if(GetHeight(t->Lchild->Rchild) > GetHeight(t->Lchild->Lchild)) { LR(t); return false; }
                if(GetHeight(t->Lchild->Rchild) < GetHeight(t->Lchild->Lchild)) { LL(t); return false; }
                if(GetHeight(t->Lchild->Rchild) == GetHeight(t->Lchild->Lchild)) { LL(t); return true; }
            }
        }

        bool Erase(const Key &target, Node *&t)
        {
            if(t == nullptr) return true;
            if(Equal(target, t->data->first))
            {
                if(t->Lchild == nullptr || t->Rchild == nullptr)
                {
                    Node* tmp = t;
                    t = (t->Lchild != nullptr) ? t->Lchild : t->Rchild;
                    delete tmp;
                    return false;
                }
                else
                {
                    Node *minR = t->Rchild, *minRfa = t;
                    while(minR->Lchild != nullptr) minRfa = minR, minR = minR->Lchild;
                    Node* minRnew = new Node(*minR->data);
                    minRnew->Lchild = minR->Lchild;
                    minRnew->Rchild = minR->Rchild;
                    minRnew->height = minR->height;
                    if(minRfa->Lchild == minR) minRfa->Lchild = minRnew;
                    else minRfa->Rchild = minRnew;
                    minR->Lchild = t->Lchild;
                    minR->Rchild = t->Rchild;
                    minR->height = t->height;
                    Node* del = t;
                    t = minR;
                    delete del;
                    if(Erase(t->data->first, t->Rchild)) return true;
                    else return Adjust(t, 1);
                }
            }
            if(Compare()(target, t->data->first))
            {
                if(Erase(target, t->Lchild)) return true;
                return Adjust(t, 0);
            }
            else
            {
                if(Erase(target, t->Rchild)) return true;
                return Adjust(t, 1);
            }
        }
        void Erase(const Key &target) { --Size; Erase(target, root); }

        void Copy(Node *&t, const Node* other)
        {
            if(other == nullptr) { t = nullptr; return; }
            t = new Node(*other->data);
            t->height = other->height;
            Copy(t->Lchild, other->Lchild);
            Copy(t->Rchild, other->Rchild);
        }

        Node* GetFirstNode() const
        {
            if(root == nullptr) return nil;
            Node* t = root;
            while(t->Lchild != nullptr) t = t->Lchild;
            return t;
        }

        Node* GetLastNode() const
        {
            if(root == nullptr) return nil;
            Node* t = root;
            while(t->Rchild != nullptr) t = t->Rchild;
            return t;
        }

        Node* GetPreNode(const Key &target, Node* t, Node *ret) const
        {
            Node *pos = t;
            while(Compare()(pos->data->first, target))
            {
                ret = pos;
                pos = pos->Rchild;
            }
            if(Equal(pos->data->first, target))
            {
                if(pos->Lchild)
                {
                    ret = pos->Lchild;
                    while(ret->Rchild != nullptr) ret = ret->Rchild;
                }
            }
            else ret = GetPreNode(target, pos->Lchild, ret);
            return ret;
        }

        Node* GetNxtNode(const Key &target, Node* t, Node *ret) const
        {
            Node *pos = t;
            while(Compare()(target, pos->data->first))
            {
                ret = pos;
                pos = pos->Lchild;
            }
            if(Equal(target, pos->data->first))
            {
                if(pos->Rchild)
                {
                    ret = pos->Rchild;
                    while(ret->Lchild != nullptr) ret = ret->Lchild;
                }
            }
            else ret = GetNxtNode(target, pos->Rchild, ret);
            return ret;
        }

    public:
        class const_iterator;
        class iterator
        {
        public:
            friend class map;

        private:
            map* ptrmap;
            Node* ptrNode;

        public:
            iterator() { ptrmap = nullptr, ptrNode = nullptr; }
            iterator(map* _ptrmap, Node* _ptrNode) :ptrmap(_ptrmap), ptrNode(_ptrNode) {}
            iterator(const iterator &other) :ptrmap(other.ptrmap), ptrNode(other.ptrNode) {}

            // iter++
            iterator operator++(int)
            {
                if(ptrmap == nullptr || ptrNode == ptrmap->nil)
                    throw invalid_iterator();
                iterator ret = *this;
                ptrNode = ptrmap->GetNxtNode(ptrNode->data->first, ptrmap->root, ptrmap->nil);
                return ret;
            }
            // ++iter
            iterator & operator++()
            {
                if(ptrmap == nullptr || ptrNode == ptrmap->nil)
                    throw invalid_iterator();
                ptrNode = ptrmap->GetNxtNode(ptrNode->data->first, ptrmap->root, ptrmap->nil);
                return *this;
            }
            // iter--
            iterator operator--(int)
            {
                if(ptrmap == nullptr || *this == ptrmap->begin())
                    throw invalid_iterator();
                iterator ret = *this;
                if(this->ptrNode == ptrmap->nil) ptrNode = ptrmap->GetLastNode();
                else ptrNode = ptrmap->GetPreNode(ptrNode->data->first, ptrmap->root, ptrmap->nil);
                return ret;
            }
            // --iter
            iterator & operator--()
            {
                if(ptrmap == nullptr || *this == ptrmap->begin())
                    throw invalid_iterator();
                if(this->ptrNode == ptrmap->nil) ptrNode = ptrmap->GetLastNode();
                else ptrNode = ptrmap->GetPreNode(ptrNode->data->first, ptrmap->root, ptrmap->nil);
                return *this;
            }

            value_type & operator*() const
            {
                if(ptrmap == nullptr || ptrNode == ptrmap->nil)
                    throw invalid_iterator();
                return *ptrNode->data;
            }

            value_type* operator->() const noexcept
            {
                if(ptrmap == nullptr || ptrNode == ptrmap->nil)
                    throw invalid_iterator();
                return ptrNode->data;
            }

            bool operator==(const iterator &rhs) const { return ptrmap == rhs.ptrmap && ptrNode == rhs.ptrNode; }
            bool operator==(const const_iterator &rhs) const { return ptrmap == rhs.ptrmap && ptrNode == rhs.ptrNode; }
            bool operator!=(const iterator &rhs) const { return ptrmap != rhs.ptrmap || ptrNode != rhs.ptrNode; }
            bool operator!=(const const_iterator &rhs) const { return ptrmap != rhs.ptrmap || ptrNode != rhs.ptrNode; }
        };
        class const_iterator
        {
        public:
            friend class map;

        private:
            const map* ptrmap;
            Node* ptrNode;

        public:
            const_iterator() { ptrmap = nullptr, ptrNode = nullptr; }
            const_iterator(const map* _ptrmap, Node* _ptrNode) :ptrmap(_ptrmap), ptrNode(_ptrNode) {}
            const_iterator(const const_iterator &other) :ptrmap(other.ptrmap), ptrNode(other.ptrNode) {}
            const_iterator(const iterator &other) :ptrmap(other.ptrmap), ptrNode(other.ptrNode) {}

            // iter++
            const_iterator operator++(int)
            {
                if(ptrmap == nullptr || ptrNode == ptrmap->nil)
                    throw invalid_iterator();
                const_iterator ret = *this;
                ptrNode = ptrmap->GetNxtNode(ptrNode->data->first, ptrmap->root, ptrmap->nil);
                return ret;
            }
            // ++iter
            const_iterator & operator++()
            {
                if(ptrmap == nullptr || ptrNode == ptrmap->nil)
                    throw invalid_iterator();
                ptrNode = ptrmap->GetNxtNode(ptrNode->data->first, ptrmap->root, ptrmap->nil);
                return *this;
            }
            // iter--
            const_iterator operator--(int)
            {
                if(ptrmap == nullptr || *this == ptrmap->cbegin())
                    throw invalid_iterator();
                const_iterator ret = *this;
                if(this->ptrNode == ptrmap->nil) ptrNode = ptrmap->GetLastNode();
                else ptrNode = ptrmap->GetPreNode(ptrNode->data->first, ptrmap->root, ptrmap->nil);
                return ret;
            }
            // --iter
            const_iterator & operator--()
            {
                if(ptrmap == nullptr || *this == ptrmap->cbegin())
                    throw invalid_iterator();
                if(this->ptrNode == ptrmap->nil) ptrNode = ptrmap->GetLastNode();
                else ptrNode = ptrmap->GetPreNode(ptrNode->data->first, ptrmap->root, ptrmap->nil);
                return *this;
            }

            value_type & operator*() const
            {
                if(ptrmap == nullptr || ptrNode == ptrmap->nil)
                    throw invalid_iterator();
                return *ptrNode->data;
            }

            value_type* operator->() const noexcept
            {
                if(ptrmap == nullptr || ptrNode == ptrmap->nil)
                    throw invalid_iterator();
                return ptrNode->data;
            }

            bool operator==(const iterator &rhs) const { return ptrmap == rhs.ptrmap && ptrNode == rhs.ptrNode; }
            bool operator==(const const_iterator &rhs) const { return ptrmap == rhs.ptrmap && ptrNode == rhs.ptrNode; }
            bool operator!=(const iterator &rhs) const { return ptrmap != rhs.ptrmap || ptrNode != rhs.ptrNode; }
            bool operator!=(const const_iterator &rhs) const { return ptrmap != rhs.ptrmap || ptrNode != rhs.ptrNode; }
        };

        map() :Size(0) { root = nullptr, nil = new Node(); }
        map(const map &other) :Size(other.Size)
        {
            nil = new Node();
            Copy(root, other.root);
        }
        map & operator=(const map &other)
        {
            if(this == &other) return *this;
            Clear(root);
            Size = other.Size;
            Copy(root, other.root);
            return *this;
        }
        ~map() { clear(); delete nil; }

        T & at(const Key &key)
        {
            Node* pos = Find(key, root);
            if(pos == nil)
                throw index_out_of_bound();
            return pos->data->second;
        }
        const T & at(const Key &key) const
        {
            Node* pos = Find(key, root);
            if(pos == nil)
                throw index_out_of_bound();
            return pos->data->second;
        }
        T & operator[](const Key &key)
        {
            Node* pos = Find(key, root);
            if(pos == nil)
            {
                ++Size;
                pos = Insert(value_type(key, T()), root);
            }
            return pos->data->second;
        }
        const T & operator[](const Key &key) const
        {
            Node* pos = Find(key, root);
            if(pos == nil)
                throw index_out_of_bound();
            return pos->data->second;
        }

        iterator begin() { return iterator(this, GetFirstNode()); }
        const_iterator cbegin() const { return const_iterator(this, GetFirstNode()); }
        iterator end() { return iterator(this, nil); }
        const_iterator cend() const { return const_iterator(this, nil); }

        bool empty() const { return Size == 0; }
        size_t size() const { return Size; }
        void clear() { Clear(root); Size = 0; }

        pair<iterator, bool> insert(const value_type &value)
        {
            Node* pos = Find(value.first);
            if(pos == nil)
            {
                ++Size;
                pos = Insert(value, root);
                iterator iter(this, pos);
                return pair<iterator, bool>(iter, true);
            }
            else
            {
                iterator iter(this, pos);
                return pair<iterator, bool>(iter, false);
            }
        }

        void erase(iterator pos)
        {
            if(pos.ptrmap != this || pos.ptrNode == nil)
                throw invalid_iterator();
            Erase(pos.ptrNode->data->first);
        }

        size_t count(const Key &key) const
        {
            if(Find(key, root) != nil) return 1;
            else return 0;
        }

        iterator find(const Key &key)
        {
            Node* pos = Find(key, root);
            return iterator(this, pos);
        }
        const_iterator find(const Key &key) const
        {
            Node* pos = Find(key, root);
            return const_iterator(this, pos);
        }
    };
} // namespace sjtu

#endif