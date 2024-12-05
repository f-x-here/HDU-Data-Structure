#include <iostream>
#include <exception>

template <typename T>
class Node
{
public:
    T data;
    Node *next;

    Node(T _data) : data(_data), next(nullptr) {}
};

template <typename T>
class mystack
{
private:
    Node<T> *topNode;

public:
    mystack() : topNode(nullptr) {}
    ~mystack()
    {
        while (!IsEmpty())
            Pop();
    }
    bool IsEmpty()
    {
        return topNode == nullptr;
    }
    void Push(T element)
    {
        Node<T> *newNode = new Node<T>(element);
        newNode->next = topNode;
        topNode = newNode;
    }
    void Pop()
    {
        Node<T> * node2Delete=topNode;
        if(!IsEmpty())
        {
            topNode=topNode->next;
            delete node2Delete;
        }
        else
        {
            throw std::out_of_range("Stack<>::pop(): empty stack");
        }
    }
    T Top()
    {
        if(IsEmpty())
        {
            throw std::out_of_range("Stack<>::top(): empty stack");
        }
        return topNode->data;
    }
};