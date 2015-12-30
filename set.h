//
// Created by k-par_000 on 19.12.2015.
//

#ifndef PERSISTENTTREE_SET_H
#define PERSISTENTTREE_SET_H




#include <vector>
#include <stdint-gcc.h>
#include <iosfwd>
#include <iostream>
#include <memory>

class set {
    typedef int value_type;
    class node {
    public:
        value_type data;
        std::shared_ptr<node> left, right;
        node();
        node(value_type value);
    };
    std::shared_ptr<node> root;
    node tail;


    std::shared_ptr<node> insert(std::shared_ptr<node> current, value_type value);
    std::shared_ptr<node> erase(std::shared_ptr<node> current, value_type value);
    std::shared_ptr<node> get_max(std::shared_ptr<node> current);
    std::shared_ptr<node> fix_branch(std::shared_ptr<node> current);
    std::shared_ptr<node> get_min(std::shared_ptr<node> current);
    std::shared_ptr<node> next(std::shared_ptr<node> current, value_type value);
    std::shared_ptr<node> prev(std::shared_ptr<node> current, value_type value);

public:
    set();
    set(set const& over);
    set&operator=(set const& over);
    class iterator {
     friend struct set;
        node* ptr;
        set* host;
    public:
        iterator();
        iterator(iterator const& over);
        iterator& operator=(iterator const& over);
        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);
        bool operator==(iterator const& over);
        bool operator!=(iterator const& over);
        value_type operator*() const;
        ~iterator();

    };
    void insert(value_type value);
    iterator erase(iterator const& iter);
    void erase(value_type value);
    void clear();
    bool empty();
    bool contains(value_type value);
    iterator begin();
    iterator end();
    ~set();

private:
    std::vector<iterator*> iterators;
    void add_iter(iterator& iter);
    void del_iter(iterator& iter);
    void del_iter();
};


#endif //PERSISTENTTREE_SET_H
