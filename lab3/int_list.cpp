#include <mach/mach_types.h>
#include "int_list.h"

int_list::int_list() {
    first = new node_t(0, nullptr, nullptr);
    last = new node_t(0, nullptr, first);
    first->next = last;
    list_size = 0;
}

int_list::node_t *int_list::get(int pos) const {
    if (pos <= list_size / 2) {
        node_t *current = first->next;
        while (pos > 0) {
            current = current->next;
            pos--;
        }

        return current;
    } else {
        node_t *current = last;
        while (pos < list_size) {
            current = current->prev;
            pos++;
        }

        return current;
    }
}

int_list::int_list(const int_list &other) : int_list() {
    list_size = other.list_size;
    auto current = other.first->next;
    while (current != other.last) {
        push_back(current->value);
        current = current->next;
    }
}

int_list::int_list(size_t count, int value) : int_list() {
    for (int i = 1; i < count; ++i) {
        push_back(value);
    }
}

int_list::~int_list() {
    clear();
    delete first;
    delete last;
}

int_list &int_list::operator=(const int_list &other) {
    int_list current(other);
    swap(current);
    return *this;
}

int &int_list::operator[](size_t pos) {
    node_t *current = get(pos);
    return current->value;
}

const int &int_list::operator[](size_t pos) const {
    node_t *pointer = get(pos);
    return pointer->value;
}

int &int_list::back() {
    return last->prev->value;
}

const int int_list::back() const {
    return last->prev->value;
}

int &int_list::front() {
    return first->next->value;
}

const int int_list::front() const {
    return first->next->value;
}

size_t int_list::size() const {
    return list_size;
}

bool int_list::empty() const {
    return (list_size == 0);
}

void int_list::clear() {
    node_t *current = first->next;
    node_t *next_node = current->next;
    while (next_node != last) {
        delete (current);
        current = next_node;
        next_node = current->next;
    }
    list_size = 0;
}

void int_list::insert(size_t pos, int new_val) {
    node_t *current = get(pos);
    node_t *new_node = new node_t(new_val, current, current->prev);
    current->prev->next = new_node;
    current->prev = new_node;
    list_size++;
}

void int_list::push_front(int new_val) {
    insert(0, new_val)
}

void int_list::push_back(int new_val) {
    insert(list_size, new_val)
}

void int_list::erase(size_t pos) {
    node_t *current = get(pos);
    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;
    list_size--;
}

void int_list::pop_front() {
    erase(0);
}

void int_list::pop_back() {
    erase(list_size - 1);
}

int_list int_list::splice(size_t start_pos, size_t count) {
    int_list result_list;
    node_t *current = get(start_pos);
    result_list.first->next = current;
    current->prev = result_list.first;
    node_t *end_1 = current->prev;

    for (int i = 0; i < count - 1; i++) {
        current = current->next;
    }

    result_list.last->prev = current;
    current->next = result_list.last;
    node_t *end_2 = current->next;

    end_1->next = end_2;
    end_2->prev = end_1;

    list_size -= count;
    result_list.list_size = count;
    return result_list;
}

int_list &int_list::merge(int_list &other) {
    if (!other.empty()) {
        last->prev->next = other.first->next;
        other.first->next->prev = last->prev;
        other.last->prev->next = last;
        last->prev = other.last->prev;

        other.first->next = other.last;
        other.last->prev = other.first;
        list_size += other.list_size;
        other.list_size = 0;
    }

    return *this;
}

void int_list::reverse() {
    if (!empty()) {
        node_t *current = first->next;
        node_t *next_node = current->next;
        while (next_node != last) {
            std::swap(current->next, current->prev);
            current = next_node;
            next_node = current->next;
        }
        first->next->prev = first;
        last->prev->next = last;
        std::swap(first->next, last->prev);
    }
}

void int_list::swap(int_list &other) {
    std::swap(first, other.first);
    std::swap(last, other.last);
    std::swap(list_size, other.list_size);
}

std::istream &operator>>(std::istream &stream, int_list &list) {
    int value = 0;
    list.clear();
    while (stream >> value) {
        list.push_back(value);
    }
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const int_list &list) {
    auto current = list.first->next;
    while (current != list.last) {
        stream << current->value << " ";
        current = current->next;
    }
    return stream;
}

