#pragma once
#include <iostream>
using std::cout;
using std::endl;

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;
    Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : data(data), prev(prev), next(next) {}
};

template <typename T>
class List {
public:
    List() = default;
    ~List();

    void insertAt(size_t pos, const T& data);
    void removeAt(size_t pos);
    size_t find(const T& data) const;
    size_t findAndReplace(const T& oldData, const T& newData);
    void reverse();
    void print() const;

private:
    Node<T>* head = nullptr;
    Node<T>* tail = nullptr;
    size_t size = 0;

    Node<T>* getNodeAt(size_t pos) const;
};

template <typename T>
List<T>::~List() {
    while (head) {
        auto tmp = head;
        head = head->next;
        delete tmp;
    }
}

template <typename T>
void List<T>::insertAt(size_t pos, const T& data) {
    if (pos > size) return;
    if (pos == 0) {
        head = new Node<T>(data, nullptr, head);
        if (!tail) tail = head;
        else head->next->prev = head;
    }
    else if (pos == size) {
        tail = new Node<T>(data, tail, nullptr);
        tail->prev->next = tail;
    }
    else {
        auto prevNode = getNodeAt(pos - 1);
        auto newNode = new Node<T>(data, prevNode, prevNode->next);
        prevNode->next->prev = newNode;
        prevNode->next = newNode;
    }
    ++size;
}

template <typename T>
void List<T>::removeAt(size_t pos) {
    if (pos >= size || !head) return;
    Node<T>* toDelete;
    if (pos == 0) {
        toDelete = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
    }
    else if (pos == size - 1) {
        toDelete = tail;
        tail = tail->prev;
        tail->next = nullptr;
    }
    else {
        toDelete = getNodeAt(pos);
        toDelete->prev->next = toDelete->next;
        toDelete->next->prev = toDelete->prev;
    }
    delete toDelete;
    --size;
}

template <typename T>
size_t List<T>::find(const T& data) const {
    size_t pos = 0;
    for (auto current = head; current; current = current->next, ++pos)
        if (current->data == data)
            return pos;
    return size_t(-1);
}

template <typename T>
size_t List<T>::findAndReplace(const T& oldData, const T& newData) {
    size_t count = 0;
    for (auto current = head; current; current = current->next)
        if (current->data == oldData) {
            current->data = newData;
            ++count;
        }
    return count > 0 ? count : size_t(-1);
}

template <typename T>
void List<T>::reverse() {
    Node<T>* current = head;
    Node<T>* temp = nullptr;
    while (current) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev;
    }
    std::swap(head, tail);
}

template <typename T>
Node<T>* List<T>::getNodeAt(size_t pos) const {
    if (pos >= size) return nullptr;
    auto current = head;
    for (size_t i = 0; i < pos; ++i)
        current = current->next;
    return current;
}

template <typename T>
void List<T>::print() const {
    for (auto current = head; current; current = current->next)
        cout << current->data << " ";
    cout << endl;
}
int main() {
    List<int> list;

    list.insertAt(0, 10);
    list.insertAt(1, 20);
    list.insertAt(1, 15);
    list.print();

    list.removeAt(1);
    list.print();

    size_t pos = list.find(20);
    if (pos != size_t(-1))
        cout << "Element found at position: " << pos << endl;
    else
        cout << "Element not found" << endl;

    size_t replaced = list.findAndReplace(20, 25);
    if (replaced != size_t(-1))
        cout << "Replaced " << replaced << " element(s)" << endl;
    else
        cout << "Element not found" << endl;
    list.print();

    list.reverse();
    list.print();

    return 0;
}
