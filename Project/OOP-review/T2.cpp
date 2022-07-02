#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
using namespace std;
#define USE_OS

class error :public exception
{
private:
    string msg;

public:
    explicit error(const char *_msg_) : msg(_msg_) {}
    const char *toString() {return msg.c_str();}
    ~error() throw() {};
};

template<class T>
class ArrayList
{
    template<class elemType>
    friend ostream& operator<<(ostream &, const ArrayList<elemType> &);

private:
    int len;
    T* array;

public:
    ArrayList(int length = 0);
    ArrayList(T*, int);
    ArrayList(const ArrayList &);
    ~ArrayList() {delete[] array;}

    int size() const {return len;}
    T &operator[](const int);
    const T &operator[](const int) const;
    ArrayList<T> operator+(const ArrayList<T> &) const;
    ArrayList<T> &operator=(const ArrayList<T> &);
    bool operator==(const ArrayList<T> &) const;
    bool operator!=(const ArrayList<T> &) const;
};

template<class T>
ostream& operator<<(ostream& os, const ArrayList<T> &obj)
{
    for(int i = 0; i < obj.size(); ++i)
        os << obj.array[i] <<' ';
    return os;
}

template<class T>
ArrayList<T>::ArrayList(int length)
{
    if(length < 0)
        throw error("invalid length");
    len = length;
    array = new T[length];
    if(array)
        for(int i = 0; i < length; ++i)
            array[i] = T();
}

template<class T>
ArrayList<T>::ArrayList(T* arr, int length)
{
    if(length < 0)
        throw error("invalid length");
    len = length;
    if(arr != NULL)
    {
        array = new T[length];
        if(array)
            for(int i = 0; i < length; ++i)
                array[i] = arr[i];
    }
}

template<class T>
ArrayList<T>::ArrayList(const ArrayList& other):len(other.len)
{
    array = new T[len];
    if(array)
        for(int i = 0; i < len; ++i)
            array[i] = other.array[i];
}

template<class T>
T &ArrayList<T>::operator[](const int index)
{
    if(index < 0 || index >= len)
        throw error("index out of bound");
    return array[index];
}

template<class T>
const T &ArrayList<T>::operator[](const int index) const
{
    if(index < 0 || index >= len)
        throw error("index out of bound");
    return array[index];
}

template<class T>
ArrayList<T> ArrayList<T>::operator+(const ArrayList<T> &other) const
{
    ArrayList<T> res(len+other.len);
    for(int i = 0; i < len; ++i)
        res[i] = array[i];
    for(int i = 0; i < other.len; ++i)
        res[i+len] = other.array[i];
    return res;
}

template<class T>
ArrayList<T> &ArrayList<T>::operator=(const ArrayList& other)
{
    if(this == &other)
        return *this;
    if(array)
    {
        delete[] array;
        array = NULL;
    }
    len = other.len;
    array = new T[len];
    if(array)
        for(int i = 0; i < len; ++i)
            array[i] = other.array[i];
    return *this;
}

template<class T>
bool ArrayList<T>::operator==(const ArrayList& other) const
{
    if(len == other.len)
    {
        for(int i = 0; i < len; ++i)
            if(!(array[i] == other.array[i]))
                return false;
        return true;
    }
    else return false;
}

template<class T>
bool ArrayList<T>::operator!=(const ArrayList& other) const
{
    return !(*this == other);
}

//#define _MAIN
#ifdef _MAIN

template<class T>
void printList(ArrayList<T> list) {
#ifdef USE_OS
    std::cout << list << "\n";
#else
    list.print();
    std::cout << "\n";
#endif
}

int main()
{
	try {
        ArrayList<char> list_err(-1);
    } catch (error &e) {
        cout << e.toString() << endl;
    }

    ArrayList<int> list(5);

    try {
        list[-1];
    } catch (error &e) {
        cout << e.toString() << endl;
    }
    try {
        list[2000];
    } catch (error &e) {
        cout << e.toString() << endl;
    }

    for (int i = 0; i < list.size(); i++) list[i] = i;
    printList(list);
    ArrayList<int> temp = list;
    printList(temp);
    temp = list;
    printList(temp);
    if (temp == list) cout << "true" << endl;

    int arr[5] = {4, 215, 12, 84, 76};
    const ArrayList<int> list_const(arr, 5);
    cout << list_const[3] << endl;
    printList(list_const);

    printList(list_const + list);

    return 0;
}

#endif