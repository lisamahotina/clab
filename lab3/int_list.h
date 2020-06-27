#include <iostream>

class int_list {
private:
    struct node_t {
        int value;
        node_t *next;
        node_t *prev;

        node_t(int new_value, node_t *new_next, node_t *new_prev) {
            value = new_value;
            next = new_next;
            prev = new_prev;
        }
    };

    size_t list_size;
    node_t *first;
    node_t *last;

    node_t *get(int pos) const;

public:
    int_list(); // O(1)
    int_list(const int_list &other); // COPY $other list O(other.size)
    int_list(size_t count, int value); // create list $count size and fill it with $value O($count)
    ~int_list();    // O(size)

    int_list &operator=(const int_list &other); // O(size + other.size())

    int &operator[](size_t pos); // O(min($pos, size - $pos))
    const int &operator[](size_t pos) const; // O(min($pos, size - $pos))

    int &back();    // last element O(1)
    const int back() const; // O(1)

    int &front();    // first element O(1)
    const int front() const; // O(1)

    void clear(); // O(size)
    size_t size() const; // O(1)
    bool empty() const; // O(1)

    void
    insert(size_t pos, int new_val); // insert element $new_val BEFORE element with index $pos O(min($pos, size - $pos))
    void push_front(int new_val); // O(1)
    void push_back(int new_val);    // O(1)

    void erase(size_t pos); // remove element with index $pos O(min($pos, size - $pos))
    void pop_front(); // O(1)
    void pop_back();  // O(1)

    int_list splice(size_t start_pos, size_t count); // splice part of list into result (not copy!) O($start_pos + $count)
    int_list &merge(int_list &other); // merge two lists, after operation $other must be valid empty list O(1)

    void reverse(); // O(size)
    void swap(int_list &other); // O(1)

    friend std::istream &operator>>(std::istream &stream, int_list &list);    // O(size)
    friend std::ostream &operator<<(std::ostream &stream, const int_list &list); // O(size)
};