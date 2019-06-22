//
// Created by Роман Агеев on 2019-06-22.
//

#ifndef YEAR2018_LIST_TESTING_LIST_HPP
#define YEAR2018_LIST_TESTING_LIST_HPP
#include <iostream>
#include <iterator>
#include <memory>
#include <assert.h>
template<typename T>
struct start_node;

template<typename T>
struct other_node;

template<typename T>
struct list;

template<typename T>
struct const_list_iterator;

template<typename T>
struct list_iterator {
  typedef T value_type;
  typedef T &reference;
  typedef std::ptrdiff_t difference_type;
  typedef T *pointer;
  typedef std::bidirectional_iterator_tag iterator_category;


  typedef start_node<T> *start_ptr;

  template<typename> friend
  struct list;
  template<typename> friend
  struct const_list_iterator;

  list_iterator() = default;
  list_iterator(list_iterator const &) = default;
  list_iterator &operator=(list_iterator const &) = default;
  list_iterator &operator++() {
    ptr = ptr->right;
    return *this;
  }

  list_iterator &operator--() {
    ptr = ptr->left;
    return *this;
  }

  const list_iterator operator++(int) {
    list_iterator tmp(*this);
    ++(*this);
    return tmp;
  }

  const list_iterator operator--(int) {
    list_iterator tmp(*this);
    --(*this);
    return tmp;
  }

  friend bool operator==(list_iterator const &in1, list_iterator const &in2) {
    return in1.ptr == in2.ptr;
  }

  friend bool operator!=(list_iterator const &in1, list_iterator const &in2) {
    return in1.ptr != in2.ptr;
  }

  reference operator*() const {
    return ptr->down()->value;
  }

  pointer operator->() const {
    return &(ptr->down()->value);
  }

 private:
  start_ptr ptr = nullptr;

  explicit list_iterator(start_ptr in) : ptr(in) {}
};

template<typename T>
struct const_list_iterator {
  typedef T value_type;
  typedef T const &reference;
  typedef std::ptrdiff_t difference_type;
  typedef T const *pointer;
  typedef std::bidirectional_iterator_tag iterator_category;

  typedef start_node<T> *start_ptr;

  template<typename> friend
  struct list;

  const_list_iterator() = default;
  const_list_iterator(const_list_iterator const &) = default;
  const_list_iterator(list_iterator<T> const &in) : ptr(in.ptr) {}
  const_list_iterator &operator=(const_list_iterator const &) = default;

  const_list_iterator &operator++() {
    ptr = ptr->right;
    return *this;
  }

  const_list_iterator &operator--() {
    ptr = ptr->left;
    return *this;
  }

  const const_list_iterator operator++(int) {
    list_iterator<T> tmp(this->ptr);
    ++(*this);
    return tmp;
  }

  const const_list_iterator operator--(int) {
    const_list_iterator tmp(*this);
    --(*this);
    return tmp;
  }

  friend bool operator==(const_list_iterator const &in1, const_list_iterator const &in2) {
    return in1.ptr == in2.ptr;
  }

  friend bool operator!=(const_list_iterator const &in1, const_list_iterator const &in2) {
    return in1.ptr != in2.ptr;
  }

  reference operator*() const {
    return ptr->down()->value;
  }

  pointer operator->() const {
    return &(ptr->down()->value);
  }
 private:
  start_ptr ptr = nullptr;

  explicit const_list_iterator(start_ptr in) : ptr(in) {}
};

template<typename T>
struct start_node {
  typedef start_node<T> *start_ptr;

  start_ptr left;
  start_ptr right;

  start_node()
      : left(this), right(this) {}
  ~start_node() = default;
  other_node<T> *down() {
    return static_cast<other_node<T> *>(this);
  };

  bool empty() {
    return (left == this) && (right == this);
  }

  void swap(start_node &in) {
    auto l1 = left;
    auto r1 = right;
    auto l2 = in.left;
    auto r2 = in.right;
    std::swap(l1->right, l2->right);
    std::swap(r1->left, r2->left);
    std::swap(*this, in);
  }
};

template<typename T>
struct other_node : start_node<T> {
  typedef start_node<T>* start_ptr;

  T value;
  other_node() = delete;
  explicit other_node(T const &in) : value(in) {};
  ~other_node() = default;
  start_ptr up() {
    return static_cast<start_ptr>(this);
  };
};

template<typename T>
struct list {
  typedef start_node<T> * start_ptr;
  typedef other_node<T> * other_ptr;

  start_node<T> end_;

  template<typename ...Args>
  start_ptr alloc_node(Args ...args) {
    return (new other_node<T>(args...))->up();
  }

  void dealloc_node(other_ptr in) {
    delete in;
  }

  void pusher(list const &in) {
    for(auto i = in.cbegin(); i != in.cend(); ++i) {
      push_back(*i);
    }
  }

 public:
  typedef list_iterator<T> iterator;
  typedef const_list_iterator<T> const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  list()  = default;
  ~list() noexcept {
    clear();
  }
  list(list const &in) : list() {
    pusher(in);
  }

  void splice(const_iterator pos, list & other, const_iterator l, const_iterator r) noexcept {
    if (l != r) {
      auto now = (pos).ptr;
      auto first = (l).ptr;
      auto last = (--r).ptr;
      first->left->right = last->right;
      last->right->left = first->left;
      now->left->right = first;
      first->left = now->left;
      last->right = now;
      now->left = last;
    }
  }

  list & operator=(list const &in) {
    list(in).swap(*this);
    return *this;
  }

  bool empty() noexcept {
    return end_.empty();
  };

  iterator end() noexcept {
    return iterator(&end_);
  }

  iterator begin() noexcept {
    return ++iterator(&end_);
  }

  const_iterator end() const noexcept {
    return cend();
  }

  const_iterator begin() const noexcept {
    return cbegin();
  }

  const_iterator cend() const noexcept {
    return const_iterator(&(const_cast<start_node<T>&>(end_)));
  }

  const_iterator cbegin() const noexcept {
    return const_iterator(end_.right);
  }

  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(cend());
  }

  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(cbegin());
  }

  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }

  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }

  reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }

  iterator insert(const_iterator i, T const &in) {
    auto tmp = alloc_node(in);
    --i;
    auto a = i.ptr;
    ++i;
    auto b = i.ptr;
    a->right = tmp;
    tmp->left = a;
    tmp->right = b;
    b->left = tmp;
    return iterator(tmp);
  }

  iterator erase(const_iterator in) noexcept {
    auto a = in.ptr;
    --in;
    auto b = in.ptr;
    ++in;
    ++in;
    auto c = in.ptr;
    b->right = c;
    c->left = b;
    dealloc_node(a->down());
    return iterator(c);
  }

  void push_back(T const &in) {
    insert(end(), in);
  }

  void push_front(T const &in) {
    insert(begin(), in);
  }

  void pop_back() noexcept {
    auto i = end();
    --i;
    erase(i);
  }

  void pop_front() noexcept {
    erase(begin());
  }

  T const &  back() const noexcept {
    return *(--cend());
  }

  T & back() noexcept {
    return *(--end());
  }

  T & front() noexcept {
    return *(begin());
  }

  T const & front() const noexcept {
    return *(cbegin());
  }

  void cout() noexcept {
    for (auto i = begin(); i != end(); ++i) {
      std::cout << (*i) << std::endl;
    }
  }

  iterator erase(const_iterator l, const_iterator r) noexcept {
    for(;l != r; l = erase(l));
    return iterator(r.ptr);
  }

  void clear() noexcept {
    for (auto i = begin(); i != end(); i = erase(i)) {}
  }

  void swap(list<T> &in) noexcept {
    end_.swap(in.end_);
  }

  friend void swap(list<T> & in1, list<T> & in2) noexcept {
    in1.swap(in2);
  }
};

#endif //YEAR2018_LIST_TESTING_LIST_HPP
