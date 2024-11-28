#include <stdexcept>
template <typename T>
class Node
{
public:
    Node(T _data) : data(_data), pre(nullptr), next(nullptr) {}
    T data;
    Node *pre, *next;
};

template <typename T>
class mydeque
{
private:
    Node<T> *head, *tail;
    int size;
public:
    mydeque()
    {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }
    bool empty()
    {
        return head == nullptr;
    }
    void push_head(T _data)
    {
        Node<T> *node = new Node<T>(_data);
        if (!empty())
        {
            head->pre = node;
            node->next = head;
            head = node;
        }
        else
        {
            head = node;
            tail = node;
        }
        size++;
    }
    void push_back(T _data)
    {
        Node<T> *node = new Node<T>(_data);
        if (!empty())
        {
            tail->next = node;
            node->pre = tail;
            tail = node;
        }
        else
        {
            head = node;
            tail = node;
        }
        size++;
    }
    void pop_head()
    {
        Node<T> *temp;
        temp = head;
        if (!empty())
        {
            head = head->next;
            if (tail != nullptr)
                head->pre = nullptr;
            delete temp;
        }
        else
        {
            throw std::runtime_error("pop empty deque!");
        }
        size--;
    }
    void pop_back()
    {
        Node<T> *temp;
        temp = tail;
        if (!empty())
        {
            tail = tail->pre;
            if (tail != nullptr)
                tail->next = nullptr;
            delete temp;
        }
        else
        {
            throw std::runtime_error("pop empty deque!");
        }
        size--;
    }
    T front()
    {
        if (empty())
        {
            throw std::runtime_error("pop empty deque!");
        }
        return head->data;
    }
    T back()
    {
        if (empty())
        {
            throw std::runtime_error("pop empty deque!");
        }
        return tail->data;
    }
    ~mydeque()
    {
        while (!empty())
        {
            pop_head();
        }
    }
};
