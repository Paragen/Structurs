#include <assert.h>
#include "set.h"

set::set(): root(nullptr)
{}

set::set(set const &over) : root(std::shared_ptr<node>(over.root))
{}

set& set::operator=(const set & over) {
    root = over.root;
    return *this;
}

set::iterator::iterator(): ptr(nullptr),host(nullptr)
{}

set::iterator::iterator(const set::iterator &over) {
    ptr = over.ptr;
    host = over.host;
    if (host != nullptr) {
        host->add_iter(*this);
    }
}


set::iterator &set::iterator::operator=(const set::iterator &over) {
    if (over.host != nullptr) {
        over.host->add_iter(*this);
    }
    if (host != nullptr) {
        host->del_iter(*this);
    }
    ptr = over.ptr;
    host = over.host;
    return *this;
}

set::iterator &set::iterator::operator++() {
    assert(host != nullptr && &(host->tail) != ptr);
    auto tmp = host->next(host->root,ptr->data);
    if (tmp == nullptr) {
        ptr = &host->tail;
    } else {
        ptr = tmp.get();
    }
    return *this;
}

set::iterator set::iterator::operator++(int i) {
    iterator buf(*this);
    ++*this;
    return buf;
}

set::iterator &set::iterator::operator--() {
    assert(host != nullptr );
    auto tmp = host->prev(host->root,ptr->data);
    assert (tmp != nullptr);
    ptr = tmp.get();
    return *this;
}

set::iterator set::iterator::operator--(int i) {
    iterator buf(*this);
    --*this;
    return buf;
}

set::value_type set::iterator::operator*() const{
    assert(host != nullptr && ptr != &(host->tail));
    return ptr->data;
}

bool set::iterator::operator==(set::iterator const& over) {
    assert(host != nullptr && host == over.host);
    return ptr == over.ptr;
}

bool set::iterator::operator!=(set::iterator const& over) {
    return !(*this == over);
}

set::iterator::~iterator() {
    if (host != nullptr) {
        host->del_iter(*this);
    }
}

void set::insert(value_type value) {
    del_iter();
    auto tmp = insert(root,value);
    root = (tmp == nullptr) ? root : tmp;
}

std::shared_ptr<set::node> set::insert(std::shared_ptr<node> current, value_type value) {
    if (current == nullptr) {
       return std::shared_ptr<node>( new node(value));
    }
    if (current -> data == value) {
        return nullptr;
    }
    if (current -> data > value) {
        auto tmp = std::make_shared<node>();
        tmp->right = current ->right;
        tmp->left = insert(current->left,value);
        if (tmp->left == nullptr) {
            return nullptr;
        } else {
            return tmp;
        }
    } else {
        auto tmp = std::shared_ptr<node>(new node(current->data));
        tmp->left = current ->left;
        tmp->right = insert(current->right,value);
        if (tmp->right == nullptr) {
            return nullptr;
        } else {
            return tmp;
        }
    }
}

set::iterator set::erase(set::iterator const& iter){
    assert(iter.host == this && iter.ptr != &tail);
    auto tmp = next(root,*iter);
    iterator buf;
    erase(*iter);
    buf.ptr = tmp.get();
    buf.host = this;
    add_iter(buf);
    return buf;
};


void set::erase(value_type value) {
    del_iter();
    root = erase(root,value);
}



std::shared_ptr<set::node> set::erase(std::shared_ptr<node> current, value_type value) {
    if (current == nullptr) {
        return nullptr;
    }

    if (current -> data == value) {
        if (current->left != nullptr && current->right != nullptr) {
            value_type max = get_max(current)->data;
            auto tmp = std::shared_ptr<node> (new node(max));
            tmp->right = current->right;
            tmp->left = fix_branch(current->left);
        }
        if (current->left != nullptr) {
            return current -> left;
        }
        if (current->right != nullptr) {
            return current -> right;
        }
        return nullptr;
    }

    if (current -> data > value) {
        auto tmp = std::shared_ptr<node>(new node(current->data));
        tmp->right = current ->right;
        tmp->left = erase(current->left,value);
        if (tmp->left == current->left) {
            return current;
        } else {
            return tmp;
        }
    } else {
        auto tmp = std::shared_ptr<node>(new node(current->data));
        tmp->left = current ->left;
        tmp->right = erase(current->right,value);
        if (tmp->right == current->right) {
            return current;
        } else {
            return tmp;
        }
    }
}

std::shared_ptr<set::node> set::get_max(std::shared_ptr<node> current) {
    while (current->right != nullptr) {
        current = current->right;
    }
    return current;
}

std::shared_ptr<set::node> set::get_min(std::shared_ptr<node> current) {
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}

std::shared_ptr<set::node> set::fix_branch(std::shared_ptr<node> current) {
    if (current->right != nullptr) {
        auto tmp = std::shared_ptr<node>(new node(current->data));
        tmp->left = current->left;
        tmp->right = fix_branch(current->right);
        return  tmp;
    } else {
        return current->left;
    }
}

void set::clear() {
    del_iter();
    root = nullptr;
}

bool set::empty() {
    return root == nullptr;
}

bool set::contains(value_type value) {
    std::shared_ptr<node> curr_root(root);
    while (curr_root != nullptr) {
        if (curr_root->data == value) {
            return true;
        }
        if (curr_root->data > value) {
            curr_root = curr_root->left;
        } else {
            curr_root = curr_root->right;
        }
    }
    return false;
}

set::iterator set::begin() {
    if (root == nullptr) {
        return end();
    }

    auto tmp = get_min(root);
    iterator buf;
    buf.host = this;
    buf.ptr = tmp.get();
    add_iter(buf);
    return buf;
}

set::iterator set::end() {
    iterator buf;
    buf.ptr = &tail;
    buf.host = this;
    add_iter(buf);
    return buf;
}

set::node::node(value_type value):data(value)
{}

set::node::node() : data(value_type())
{}

std::shared_ptr<set::node> set::next(std::shared_ptr<node> current, value_type value) {
    if (current == nullptr) {
        return nullptr;
    }
    if (current->data == value) {
        if (current->right != nullptr) {
            return get_min(current->right);
        }
        return nullptr;
    }
    if (current->data > value) {
       auto tmp = next(current->left,value);
        if (tmp == nullptr) {
            return current;
        }
        return tmp;
    } else {
        return next(current->right, value);
    }
}

std::shared_ptr<set::node> set::prev(std::shared_ptr<node> current, value_type value) {
    if (current == nullptr) {
        return nullptr;
    }
    if (current->data == value) {
        if (current->left != nullptr) {
            return get_max(current->left);
        }
        return nullptr;
    }
    if (current->data < value) {
        auto tmp = next(current->right,value);
        if (tmp == nullptr) {
            return current;
        }
        return tmp;
    } else {
        return next(current->left, value);
    }
}

set::~set() {
    clear();
}

void set::add_iter(set::iterator &iter) {
    iterators.push_back(&iter);
}

void set::del_iter(set::iterator &iter) {
    for (std::size_t i = 0; i < iterators.size(); i++) {
        if (&iter == iterators[i]) {
            iterators[i] = iterators.back();
            iterators.pop_back();
            return;
        }
    }
}

void set::del_iter() {
    for (std::size_t i = 0; i < iterators.size(); i++) {
        iterators[i]->host = nullptr;
    }
    iterators.clear();
}
