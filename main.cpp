#include <iostream>
#include <stdlib.h>
#include <memory>
#include "set.h"
using namespace std;
bool foo() {
    return true;
}
void bar(std::shared_ptr<int> ptr,bool flag){

}

int main() {
    bar(std::shared_ptr<int>(new int()),foo());
    bar(std::make_shared<int>(),foo());
    set a;
    for (int i = 0; i < 1000; i++) {
        a.insert(i);
    }
    for (int i = 0; i< 1000; i++) {
        a.erase(i);
    }
    std::cout << a.empty() << std::endl;
    for (int i = 500; i < 1000; i++) {
        a.insert(i);
    }
    for (int i = 499; i > 0; i--) {
        a.insert(i);
    }
    for (set::iterator iter = a.begin(); iter != a.end(); iter++) {
        std::cout << *iter << std::endl;
    }
    set b(a);
    b.erase(500);
    std::cout << a.contains(500) << b.contains(500) << std::endl;
    set::iterator iter;
    iter = a.begin();
    a.erase(a.erase(iter));
    std:: cout << a.contains(1) << a.contains(2) << std::endl;
    //a.erase(iter);
    iter = a.begin();
    b.erase(iter);

}