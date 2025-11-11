#pragma once
#include <iostream>

template<typename T>
class List
{
private:
    struct Node
    {
        T data;
        Node* prev;

        Node(const T& data, Node* prevNode):data(value),prev(prevNode){}
    };

    Node* first {nullptr};
    Node* last {nullptr};

    void push_back_helper(Node*& first, Node*& last, const T& value)
    {
        last = new Node(value, last);
        if (first == nullptr)
        {
            first = last;
        }
    }

    void copy(const List& other){
        // Copy last
        Node* newLast = new Node(*other.last->data,nullptr);
        Node* temp = other.last->prev;
        Node* newTemp = newLast;

        // Copy last.prev
        // newLast->prev = new Node(temp->data,nullptr);
        // Node* tempNew = newLast->prev;
        // //Copy last.prev.prev
        // temp = temp->prev; //other.last->prev->prev;
        // // newLastLast = new Node(temp->data, nullptr);
        // tempNew->prev = new Node(temp->data, nullptr);       

        while(temp) {
            newTemp->prev = new Node(temp->data,nullptr);
            temp = temp->prev;
            newTemp = newTemp->prev;
        }
        Node* newBegin = newTemp;

        free();
        last = newLast;
        first = newBegin;
    }
    
    void free() {
        while (last) {
            Node* temp = last;
            last = last->prev;
            delete temp;
        }
        first = nullptr;
    };
    
public:
    List():first(nullptr),last(nullptr){}
    List(const List& other);
    List(vector<T> values);
    ~List()
    {
        free();
    }

    List& operator=(const List& other);

    void push_back(const T& value)
    {
        // if(last == nullptr) // Empty list
        // {
        //     last = new Node(value , nullptr);
        //     first = last;
        // }
        // else
        // {
        //     last = new Node(value, last);
        // }

        // last = new Node(value, last);
        // if (first == nullptr)
        // {
        //     first = last;
        // }

        push_back_helper(first, last, value);
    }

    T pop()
    {
       T tempdata = last->data;
       Node* tempptr = last;
       last = last->prev;
       delete tempptr;
       
       return tempdata;
    }
    
};

