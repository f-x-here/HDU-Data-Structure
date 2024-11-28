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
        while (!isEmpty())
            pop();
    }
    bool isEmpty()
    {
        return topNode == nullptr;
    }
    void push(T element)
    {
        Node<T> *newNode = new Node<T>(element);
        newNode->next = topNode;
        topNode = newNode;
    }
    void pop()
    {
        Node<T> * node2Delete=topNode;
        if(!isEmpty())
        {
            topNode=topNode->next;
            delete node2Delete;
        }
        else
        {
            throw std::out_of_range("Stack<>::pop(): empty stack");
        }
    }
    T top()
    {
        if(isEmpty())
        {
            throw std::out_of_range("Stack<>::top(): empty stack");
        }
        return topNode->data;
    }
};