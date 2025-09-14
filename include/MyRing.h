#ifndef MYRING_H
#define MYRING_H

template <typename T> class RingNode {
public:
  T data;
  RingNode *next;
  RingNode *prev;
  RingNode() {
    next = nullptr;
    prev = nullptr;
  }
  RingNode(T data) {
    this->data = data;
    next = nullptr;
    prev = nullptr;
  }
};

template <typename T> class MyRing {
  RingNode<T> *head;
  int ring_size;

public:
  MyRing() {
    head = nullptr;
    ring_size = 0;
  }
  MyRing(const MyRing &other) {
    RingNode<T> *temp = other.head;
    for (int i = 0; i < other.ring_size; i++) {
      push(temp->data);
      temp = temp->next;
    }
    delete temp;
  }
  ~MyRing() {
    while (ring_size)
      pop_head();
  }
  void push(T data) {
    RingNode<T> *newNode = new RingNode<T>(data);
    if (ring_size == 0) {
      head = newNode;
      head->next = head;
      head->prev = head;
    } else {
      newNode->prev = head->prev;
      head->prev->next = newNode;
      newNode->next = head;
      head->prev = newNode;
    }
    ring_size++;
  }
  void pop_head() {
    if (ring_size == 0)
      throw "Ring is empty";
    else if (ring_size == 1) {
      delete head;
      head = nullptr;
    } else {
      RingNode<T> *temp = head;
      head->prev->next = head->next;
      head->next->prev = head->prev;
      head = head->next;
      delete temp;
    }
    ring_size--;
  }
  T get_data() {
    if (ring_size == 0)
      throw "Ring is empty";
    return head->data;
  }
  void next_node() {
    if (ring_size == 0)
      throw "Ring is empty";
    head = head->next;
  }
  void prev_node() {
    if (ring_size == 0)
      throw "Ring is empty";
    head = head->prev;
  }
  bool empty() {
    if (ring_size)
      return false;
    return true;
  }
  int size() { return ring_size; }
  void clean() {
    while (ring_size)
      pop_head();
    head = nullptr;
  }
  T &operator[](const int index) {
    RingNode<T> *temp = head;
    if (index > 0) {
      for (int i = 0; i < index; i++)
        temp = temp->next;
    }
    if (index < 0) {
      for (int i = 0; i > index; i--)
        temp = temp->prev;
    }
    return temp->data;
  }
};

#endif