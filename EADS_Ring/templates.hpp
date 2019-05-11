#ifndef templates_hpp
#define templates_hpp

#include <stdio.h>

// With iterator
// Iterator jump links by number function to jump 2 elements
// Define * dereference
// Define begin
// Define ++, !=, +=2(?), --(since it's a ring)
template<typename Key>
class Ring{ // double linked, with iterator
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
        void print(){
            std::cout << key << std::endl;
        }
    };
    
    Node* head;
    int length;
    
    bool insert_between(Key k, Node* first, Node* second);      //done
    bool insert_around(Key target, Key k, int n, bool front);   //done
public:
    struct Iterator{
        Node* current;
        int off; // offset from first element

        Iterator():current(nullptr){}
        Iterator(Node* ptr):current(ptr),off(0){}
        
        
        Node& operator*(){ return *current;}
        
        int offset(){ return off > 0 ? off : -off;}
        
        Iterator& operator++(int){
            current = current->next;
            off++;
            return *this;
        }
        Iterator& operator--(int){
            current = current->prev;
            off--;
            return *this;
        }
        
        bool operator==(const Iterator& x){ return current == x.current;}
        bool operator!=(const Iterator& x){ return current != x.current;}
    };
    
    Ring():head(nullptr),length(0){}
    Ring(Ring &);
    //~Ring();
    
    Iterator begin();   //done
    
    void push_back(Key);                                //done
    void push_front(Key);                               //done
    bool insert_after(Key after, Key k, int n=0);       //done
    bool insert_before(Key before, Key k, int n=0);     //done
    
    bool remove(Key, int n=1);      // done
    bool remove_all(Key);           // done
    
    bool find(Key);
    void print();
    bool is_empty();
    int size(){return length;}
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



// Begin function, returning iterator with head for ring
template <class Key>
typename Ring<Key>::Iterator Ring<Key>::begin(){
    return Ring<Key>::Iterator(head);
}


// Inserts a new key between 2 nodes
// Return 1 if given nodes aren't consecutive
template<class Key>
bool Ring<Key>::insert_between(Key k, Node* first, Node* second){
    if(first->next != second || second->prev != first)
        return 1;
    second->prev = first->next = new Node(k,first,second);
    return 0;
}

// Appends at the end of cycle
template <class Key>
void Ring<Key>::push_back(Key k){
    if(!head){
        head = new Node(k);
        head->prev = head;
        head->next = head;
    }
    else{
        this->insert_between(k, head->prev, head);
    }
    length++;
}

// Adds element to the front
template <class Key>
void Ring<Key>::push_front(Key k){
    this->push_back(k);
    head = head->prev;
}

// Inserts new node in before or after the given node
template <class Key>
bool Ring<Key>::insert_around(Key target, Key k, int n, bool front){
    for(Ring<Key>::Iterator it = this->begin(); it.offset() != this->size(); it++){
        if((*it).key == target){
            if(n==0){
                if(front)
                    this->insert_between(k, it.current, (*it).next);
                else
                    this->insert_between(k, (*it).prev, it.current);
                this->length++;
                return 0;
            }
            n--;
        }
    }
    return 1;
    
}

// Inserts a node after n-th node with given key, default n=0 for first element
template <class Key>
bool Ring<Key>::insert_after(Key after, Key k, int n){
    return this->insert_around(after, k, n, 1);
}

// Inserts a node before n-th node with given key, default n=0 for first element
template <class Key>
bool Ring<Key>::insert_before(Key after, Key k, int n){
    return this->insert_around(after, k, n, 0);
}

// Removes a node with given key
template<class Key>
bool Ring<Key>::remove(Key x, int n){
    if(this->size() == 1 && head->key == x){
        delete head;
        head = nullptr;
        length--;
        return 0;
    }
    for(Ring<Key>::Iterator it = this->begin(); it.offset() != this->size(); it++){
        if((*it).key == x){
            if(n==0){
                (*it).prev->next = (*it).next;
                (*it).next->prev = (*it).prev;
                length--;
                delete it.current;
                return 0;
            }
            n--;
        }
    }
    return 1;
}

// Removes all nodes with given key
// Doesn't use iterator because we are editing the ring while going over which creates skips
template<class Key>
bool Ring<Key>::remove_all(Key x){
    bool f = 0;
    Node* tmp1 = head;
    Node* tmp2;
    int off = 0;
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
            f = 1;
        }
        else{
            off++;
            tmp1 = tmp1->next;
        }
        
    }
    return f;
}


template <class Key>
void Ring<Key>::print(){
    for(Ring<Key>::Iterator it = this->begin(); it.offset() != this->size(); it++){
        std::cout << (*it).key << std::endl;
    }
}






template<typename Key>
void split(const Ring<Key>& source,
           Ring<Key>& result1, bool dir1, int len1,
           Ring<Key>& result2, bool dir2, int len2){
    // every second element in direction indicated of length len
    // res1 starts from 0 element goes to right appends front
    // res2 starts from 1 element goes to right appends back
    // Dir indicates direction which the links are going
}
#endif /* templates_hpp */
