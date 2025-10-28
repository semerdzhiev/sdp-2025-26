#pragma once

template <typename Type>
class list {
public:

    /// Thrown when an operation requires the list to have at least one
    /// element, but was performed for an empty array.
    class empty_list_error : public std::logic_error {
    public:
        empty_list_error()
            : std::logic_error("Operation was performed on an empty list")
        {}
    };

    /// Represents one node in a linked list
    class node {
    public:
        node* next = nullptr;
        Type value = Type();

        node() = default;

        node(Type value, node* next = nullptr)
            : value(value), next(next)
        {
            // Nothing to do here
        }

        bool has_next() const noexcept {
            return next != nullptr;
        }
    };

    class chain_operations {
    public:
        /// Release the memory allocated for a chain of nodes
        static void free(node* head) {
            while(head) {
                node* temp = head;
                head = head->next;
                delete temp;
            }
        }

        /// Clone a chain of nodes
        static node* clone(node* head) {
            node* new_head = nullptr;

            if( head ) {
                new_head = new list<int>::node(head->value);

                node* clone = new_head;

                for(head = head->next; head; head = head->next) {
                    try {
                        clone->next = new node(head->value);
                    }
                    catch(std::bad_alloc& ) {
                        free(new_head);
                        return nullptr;
                    }
                    clone = clone->next;
                }
            }

            return new_head;
        }

        /// Returns true if two chains are of identical size and element values
        static bool identical(node* left, node* right) {
            while(left) {
                if( ! right || left->value != right->value)
                    return false;

                left = left->next;
                right = right->next;
            }

            return ! right;
        }
    };

private:
    node* m_head = nullptr;
    size_t m_size = 0;

public:
    list() = default;

    ~list() {
        chain_operations::free(m_head);
    }

    list(const list& other) {
        m_head = chain_operations::clone(other.m_head);
        m_size = other.m_size;
    }

    list& operator=(const list& other) {
        if(this != &other) {
            node* copy = chain_operations::clone(other.m_head);
            chain_operations::free(m_head);
            m_head = copy;
        }

        return *this;
    }

    list(list&& other) 
        : m_head(other.m_head), m_size(other.m_size)
    {
        other.m_head = nullptr;
        other.m_size = 0;
    }

    list& operator=(list&& other) {
        assert(this != &other);

        chain_operations::free(m_head);

        m_head = other.m_head;
        m_size = other.m_size;
        other.m_head = nullptr;
        other.m_size = 0;       

        return *this;
    }

    size_t size() const {
        return m_size;
    }

    Type& front() {
        if( ! m_head)
            throw empty_list_error();

        return m_head->value;
    }

    const Type& front() const {
        if( ! m_head)
            throw empty_list_error();

        return m_head->value;
    }

    void push_front(const Type& value) {
        m_head = new node(value, m_head);
        ++m_size;
    }

    void pop_front() {
        if( ! m_head)
            throw empty_list_error();

        node* temp = m_head;
        m_head = m_head->next;
        --m_size;
        delete temp;
    }

    bool operator==(const list& other) {
        return chain_operations::identical(m_head, other.m_head);
    }

};
