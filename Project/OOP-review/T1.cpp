#include <cstdio>
#include <iostream>
using namespace std;

template<class T>
class LinkedList
{
private:
    struct node
    {
        T data;
        node* prev;
        node* next;
    };

protected:
    int length;
    node *head,*tail;  

public:
    LinkedList();
    LinkedList(const LinkedList<T>& L);
    virtual ~LinkedList();
    
    void pushFront(T x);
    void pushBack(T x);
    T popFront();
    T popBack();
    int size() const {return length;}
    void print() const;

    virtual const char *name() const {return "LinkedList";}
    virtual T peak() const {return T();}
    virtual T pop() {return T();}
    virtual void push(T val) {};
};

#ifndef LINKEDLIST_IMPLEMENTATION
template<class T>
LinkedList<T>::LinkedList()
{
    head = new node;
    tail = new node;
    head->next = tail;
    head->prev = NULL;
    tail->next = NULL;
    tail->prev = head;
    length = 0;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& L)
{
    head = new node;
    tail = new node;
    head->prev = NULL;
    head->next = tail;
    tail->prev = head;
    length = L.length;
    node *temp = L.head;
    while(temp != L.tail->prev)
    {
        temp = temp->next;
        tail->data = temp->data;
        node *p = new node;
        p->prev = tail;
        tail->next = p;
        tail = p;
    }
    tail->next = NULL; 
}

template<class T>
LinkedList<T>::~LinkedList()
{
    if(length == 0)
    {
        delete head;
        delete tail;
        head = NULL;
        tail = NULL;
    } 
    else
    {
        while(head->next != NULL)
        {
            node *temp = head;
            head = head->next;
            delete temp;
            temp = NULL;
        }
        delete head;
        head = NULL;
    }
}

//在链表头插入结点，值为x
template<class T>
void LinkedList<T>::pushFront(T x)
{
    node *add = new node;
    add->data = x;
    add->next = head->next;
    add->prev = head;
    head->next->prev = add;
    head->next = add;
    length++;
}

//在链表尾插入结点，元素为x
template<class T>
void LinkedList<T>::pushBack(T x)
{
    node *add = new node;
    add->data = x;
    add->next = tail;
    add->prev = tail->prev;
    tail->prev->next = add;
    tail->prev = add;
    length++;
}

//在链表头删除结点，返回元素
template<class T>
T LinkedList<T>::popFront()
{
    if(length == 0)
        return T();
    node *temp = head->next;
    T ans = temp->data;
    head->next = temp->next;
    head->next->prev = temp->prev;
    delete temp;
    temp = NULL;
    length--;
    return ans;
}

//在链表尾段删除结点，返回元素
template<class T>
T LinkedList<T>::popBack()
{
    if(length == 0)
        return T();
    node *temp = tail->prev;
    T ans = temp->data;
    tail->prev = temp->prev;
    tail->prev->next = temp->next;
    delete temp;
    temp = NULL;
    length--;
    return ans;
}

//输出链表
template<class T>
void LinkedList<T>::print() const
{
    if(length == 0) return;
    node *temp = head->next;
    while(temp != tail->prev)
    {
        cout << temp->data << ' ';
        temp = temp->next; 
    }
    cout << temp->data << endl;
}
#endif

template<class T>
class Stack:public LinkedList<T>
{
    public:
        Stack():LinkedList<T>() {}
        const char *name() const {return "Stack";};
        T peak() const {return this->tail->prev->data;}
        T pop() {return this->popBack();}
        void push(T val) {this->pushBack(val);}
};

template<class T>
class Queue:public LinkedList<T>
{
    public:
        Queue():LinkedList<T>() {}
        const char *name() const {return "Queue";};
        T peak() const {return this->head->next->data;}
        T pop() {return this->popFront();}
        void push(T val) {this->pushBack(val);}
};

//#define _MAIN
#ifdef _MAIN
int main() {
    LinkedList<int> * list = new LinkedList<int>;
    LinkedList<int> * stack = new Stack<int>;
    LinkedList<int> * queue = new Queue<int>;

    cout << "LinkedList" << endl;
    list->pushFront(1);
    list->pushBack(2);
    list->pushBack(3);
    list->pushFront(4);
    list->print();
    list->popFront();
    list->popBack();
    list->print();
    cout << list->size() << endl;

    cout << stack->name() << endl;
    stack->push(1);
    stack->push(2);
    stack->push(3);
    stack->push(4);
    cout << stack->peak() << endl;
    stack->pop();
    cout << stack->pop() << endl;
    cout << stack->peak() << endl;

    cout << queue->name() << endl;
    queue->push(1);
    queue->push(2);
    queue->push(3);
    queue->push(4);
    cout << queue->peak() << endl;
    queue->pop();
    cout << queue->pop() << endl;
    cout << queue->peak() << endl;

    delete list;
    delete stack;
    delete queue;

    return 0;
}
#endif
