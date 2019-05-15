#ifndef templates_hpp
#define templates_hpp

#include <stdio.h>
#include <exception>


template<typename Key>
class Ring{
private:
    struct Node{
        Key key;
        Node* next;
        Node* prev;
        Node():next(nullptr),prev(nullptr){}
        Node(Key k, Node* p=nullptr, Node* n=nullptr){
            key = k;
            prev = p;
            next = n;
        }
        Node(Node const &x){
            key = x.key;
            next = nullptr;
            prev = nullptr;
        }
    };
    
    Node* head;
    int length;
    
    void remove(Node*);
    Node* ptr_find(Key,int n=0);
    bool insert_between(Key k, Node* first, Node* second);
    bool insert_around(Key target, Key k, int n, bool front);
public:
    struct Iterator{
        Node* node;
        int off;        // offset from first element

        Iterator():node(nullptr){}
        Iterator(Node* ptr):node(ptr),off(0){}
        
        
        Node& operator*(){ return *node;}
        
        int offset(){ return off > 0 ? off : -off;}
        
        Iterator& operator++(int){
            node = node->next;
            off++;
            return *this;
        }
        Iterator& operator--(int){
            node = node->prev;
            off--;
            return *this;
        }
        
        bool operator==(const Iterator& x){ return node == x.node;}
        bool operator!=(const Iterator& x){ return node != x.node;}
    };
    
    Ring():head(nullptr),length(0){}
    Ring(Ring &);
    ~Ring();
    
    // Iterator functions
    Iterator begin() const;
    Iterator iter_find(Key, int n=0) const;
    
    // Addition
    bool push_back(Key);
    bool push_front(Key);
    bool insert_after(Key after, Key k, int n=0);
    bool insert_before(Key before, Key k, int n=0);
    
    // Removal
    void remove(Key, int n=0);
    int remove_all(Key);
    void erase();
    
    // Qualify of life
    int count(Key);
    void shift_head(int x);
    bool find(Key);
    void print();
    bool is_empty(){return head == nullptr;}
    int size() const{return length;}
};

// Copy constructor using iterator
template <class Key>
Ring<Key>::Ring(Ring<Key> &x){
    head = nullptr;
    length = 0;
    for(Ring<Key>::Iterator it = x.begin(); it.offset() < x.size(); it++){
        this->push_back((*it).key);
    }
}

// Destructor, calls erase function
template <class Key>
Ring<Key>::~Ring(){
    this->erase();
    delete head;
}

// Counts occurances of a given Key
template <class Key>
int Ring<Key>::count(Key k){
    int c = 0;
    for(Ring<Key>::Iterator it = this->begin(); it.offset() < this->size(); it++){
        if( (*it).key == k)
            c++;
    }
    return c;
}

// Shifts the 'head' element of the ring in position
// Takes any integer but it will not rotate more than 1 full cycle
template <class Key>
void Ring<Key>::shift_head(int x){
    bool reverse;
    if(x < 0){
        reverse = 1;
        x = -x;
    }
    else{
        reverse = 0;
    }
    x = x % size();
    for(int i=0;i < x;i++){
        if(reverse){
            head = head->prev;
        }
        else{
            head = head->next;
        }
    }
}

// Begin function, returning iterator with head for ring
template <class Key>
typename Ring<Key>::Iterator Ring<Key>::begin() const{
    return Ring<Key>::Iterator(head);
}

// Function to get iterator starting with n-th occurance of a node with given key
// Throws exception if there isn't a node with given key
template <class Key>
typename Ring<Key>::Iterator Ring<Key>::iter_find(Key k,int n) const{
    for(Ring<Key>::Iterator it = this->begin(); it.offset() < size(); it++){
        if((*it).key == k){
            if(n == 0){
                it.off = 0;
                return it;
            }
            n--;
        }
    }
    throw std::invalid_argument("Element of given parameters not found");
}

// Private, returns a pointer to element with given key
// Makes use of iter_find function
template <class Key>
typename Ring<Key>::Node* Ring<Key>::ptr_find(Key k,int n){
    return this->iter_find(k,n).node;
}

// Returns true if there is a given Key in the ring
template <class Key>
bool Ring<Key>::find(Key k){
    try{
        ptr_find(k);
        return 1;
    }catch(std::invalid_argument){
        return 0;
    }
}

// Inserts a new key between 2 nodes
// Return 1 if given nodes aren't consecutive, 0 if everything is ok
template<class Key>
bool Ring<Key>::insert_between(Key k, Node* first, Node* second){
    if(first->next != second || second->prev != first)
        return 1;
    second->prev = first->next = new Node(k,first,second);
    return 0;
}

// Appends at the end of cycle
template <class Key>
bool Ring<Key>::push_back(Key k){
    if(!head){
        head = new Node(k);
        head->next = head;
        head->prev = head;
        length++;
        return 0;
    }
    else{
        length++;
        return this->insert_between(k, head->prev, head);
    }
}


// Adds element to the front
template <class Key>
bool Ring<Key>::push_front(Key k){
    bool b = this->push_back(k);
    head = head->prev;
    return b;
}


// Inserts new node in before or after the given node
template <class Key>
bool Ring<Key>::insert_around(Key target, Key k, int n, bool front){
    Node* tmp = ptr_find(target,n);
    length++;
    if(front)
        return insert_between(k,tmp,tmp->next);
    else
        return insert_between(k,tmp->prev,tmp);
}

// Inserts a node after n-th node with given key, default n=0 for first element
template <class Key>
bool Ring<Key>::insert_after(Key after, Key k, int n){
    return insert_around(after, k, n, 1);
}

// Inserts a node before n-th node with given key, default n=0 for first element
template <class Key>
bool Ring<Key>::insert_before(Key after, Key k, int n){
    return insert_around(after, k, n, 0);
}


// Removes using node pointer
template <class Key>
void Ring<Key>::remove(Node* x){
    if(x->next == x && x == x->prev){
        delete x;
        length--;
    }
    else{
        x->prev->next = x->next;
        x->next->prev = x->prev;
        delete x;
        length--;
    }
}

// Removes n-th node with given key
// ptr_find function will raise an exception if arguments are invalid
template<class Key>
void Ring<Key>::remove(Key k, int n){
    remove(ptr_find(k,n));
}

// Removes all nodes with given key
template<class Key>
int Ring<Key>::remove_all(Key x){
    bool f = 0;
    Node* tmp1 = head;
    Node* tmp2;
    int off = 0;
    int removed = 0;
    while(off < length){
        if(tmp1->key == x){
            tmp2 = tmp1;
            tmp1 = tmp1->next;
            tmp2->prev->next = tmp2->next;
            tmp2->next->prev = tmp2->prev;
            if(tmp2 == head)
                head = tmp2->next;
            delete tmp2;
            length--;
            removed++;
            f = 1;
        }
        else{
            off++;
            tmp1 = tmp1->next;
        }
    }
    return removed;
}

// Removes all elements from the list
template <class Key>
void Ring<Key>::erase(){
    Node* tmp1 = head;
    Node* tmp2;
    head = nullptr;
    while(length != 0){
        tmp2 = tmp1;
        tmp1 = tmp1->next;
        delete tmp2;
        length--;
    }
}


// Prints the contents of the ring starting from head element
template <class Key>
void Ring<Key>::print(){
    for(Ring<Key>::Iterator it = this->begin(); it.offset() != this->size(); it++){
        std::cout << (*it).key << std::endl;
    }
}


// Splits source rings into 2 given rings
// Res1 starts from 0 and get every even element appended according to given direction
// Res1 starts from 1 and get every odd element appended according to given direction
// Dir == True is clockwise
template<typename Key>
void split(const Ring<Key>& source,
           Ring<Key>& result1, bool dir1, int len1,
           Ring<Key>& result2, bool dir2, int len2){
    result1.erase();
    result2.erase();
    
    for(typename Ring<Key>::Iterator it = source.begin(); it.offset() < (len1>len2 ? 2 * len1 - 1 : 2 * len2); it++){
        if(it.offset()%2==0 && result1.size() < len1){
            if(dir1)
                result1.push_back((*it).key);
            else
                result1.push_front((*it).key);
        }
        if(it.offset()%2==1 && result2.size() < len2){
            if(dir2)
                result2.push_back((*it).key);
            else
                result2.push_front((*it).key);
        }
    }
}
#endif /* templates_hpp */
