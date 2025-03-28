#include "set.h"
#include "node.h"

int Set::Node::count_nodes = 0;

/*****************************************************
 * Implementation of the member functions             *
 ******************************************************/

/*
 *  Default constructor :create an empty Set
 */
int Set::get_count_nodes() {
    return Set::Node::count_nodes;
}

/*
 *  Default constructor :create an empty Set
 */
Set::Set() : head{ new Node() }, tail{ new Node(0, nullptr, head) }, counter { 0 } {
    head->next = tail;
}

/*
 *  Conversion constructor: convert val into a singleton {val}
 */
Set::Set(int val) : Set{} {  // create an empty list
    insert_node(head, val);
}

/*
 * Constructor to create a Set from a sorted vector of ints
 * Create a Set with all ints in sorted vector list_of_values
 */
Set::Set(const std::vector<int>& list_of_values) : Set{} {  // create an empty list
    auto itr = list_of_values.begin();
    Node* ptr = head;
    while (itr != list_of_values.end()) {
        insert_node(ptr, *itr);
        ptr = ptr->next;
        itr++;
    }
}

/*
 * Copy constructor: create a new Set as a copy of Set S
 * \param S Set to copied
 * Function does not modify Set S in any way
 */
Set::Set(const Set& S) : Set{} {  // create an empty list
    Node* p_other = S.head;
    Node* p_this = head;
    while ((p_other = p_other->next) != S.tail) {
        insert_node(p_this, p_other->value);
        p_this = p_this->next;
    }
}

/*
 * Transform the Set into an empty set
 * Remove all nodes from the list, except the dummy nodes
 */
void Set::make_empty() {
    Node* ptr = head->next;
    while (ptr = ptr->next) {
        remove_node(ptr->prev);
    }
    counter = 0;
    head->next = tail;
    tail->prev = head;
}

/*
 * Destructor: deallocate all memory (Nodes) allocated for the list
 */
Set::~Set() {
    make_empty();
    delete head;
    delete tail;
}

/*
 * Assignment operator: assign new contents to the *this Set, replacing its current content
 * \param S Set to be copied into Set *this
 * Call by valued is used
 */
Set& Set::operator=(Set S) {
    std::swap(head, S.head);
    std::swap(tail, S.tail);
    counter = S.counter;
    return *this;
}

/*
 * Test whether val belongs to the Set
 * Return true if val belongs to the set, otherwise false
 * This function does not modify the Set in any way
 */
bool Set::is_member(int val) const {
    Node* ptr = head;
    while((ptr = ptr->next) != tail && ptr->value <= val)
    {
        if (ptr->value == val)
            return true;
    }
    return false;
}

/*
 * Test whether Set *this and S represent the same set
 * Return true, if *this has same elemnts as set S
 * Return false, otherwise
 */
bool Set::operator==(const Set& S) const {
    if (counter != S.counter)
        return false;
    Node* p_this = head->next;
    Node* p_other = S.head->next;
    do {
        if (p_this->value != p_other->value)
            return false;
        p_other = p_other->next;
    } while (p_this = p_this->next);
    return true;
}

/*
 * Three-way comparison operator: to test whether *this == S, *this < S, *this > S
 * Return std::partial_ordering::equivalent, if *this == S
 * Return std::partial_ordering::less, if *this < S
 * Return std::partial_ordering::greater, if *this > S
 * Return std::partial_ordering::unordered, otherwise
 */
std::partial_ordering Set::operator<=>(const Set& S) const {

    //Check if equal
    if (counter == S.counter) {
        if(*this == S)
            return std::partial_ordering::equivalent;
        return std::partial_ordering::unordered;
    }

    Node* p_short = S.head->next;
    Node* p_short_tail = S.tail;
    Node* p_long = head->next;
    Node* p_long_tail = tail;

    // Check which set is shorter
    if (counter < S.counter) {
        std::swap(p_short, p_long);
        std::swap(p_short_tail, p_long_tail);
    }

    while (p_short != p_short_tail && p_long != p_long_tail) {
        if (p_short->value == p_long->value) {
            p_short = p_short->next;
            p_long = p_long->next;
        }
        else if (p_short->value < p_long->value) { //If value dossent exist in both sets
            return std::partial_ordering::unordered;
        }
        else {
            p_long = p_long->next;
        }
    }

    if (p_short == p_short_tail) { //If the short one has been gone through completly aka the numbers are not bigger than in the short one then the long one
        if (counter < S.counter) //If this set is smaller then it is a subset of S otherwise the revers
            return std::partial_ordering::less;
        return std::partial_ordering::greater;
    }
    return std::partial_ordering::unordered;
}

/*
 * Modify Set *this such that it becomes the union of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator+=(const Set& S) {
    Node* p_this = head->next;
    Node* p_other = S.head->next;

    while (p_this != tail && p_other != S.tail) {
        if (p_this->value == p_other->value) {
            p_this = p_this->next;
            p_other = p_other->next;
        }
        else if (p_other->value < p_this->value) {
            insert_node(p_this->prev, p_other->value);
            p_other = p_other->next;
        }
        else {
            p_this = p_this->next;
        }
    }
    while (p_other != S.tail) {
        insert_node(p_this->prev, p_other->value);
        p_other = p_other->next;
    }
    return *this;
}

/*
 * Modify Set *this such that it becomes the intersection of *this with Set S
 * Set *this is modified and then returned
 */
Set& Set::operator*=(const Set& S) {
    Node* p_this = head->next;
    Node* p_other = S.head->next;

    while (p_this != tail && p_other != S.tail) {
        if (p_this->value == p_other->value) {
            p_this = p_this->next;
            p_other = p_other->next;
        }
        else if (p_this->value < p_other->value) {
            p_this = p_this->next;
            remove_node(p_this->prev);
        }
        else {
            p_other = p_other->next;
        }
    }
    while (p_this != tail) {
        p_this = p_this->next;
        remove_node(p_this->prev);
    }

    return *this;
}

/*
 * Modify Set *this such that it becomes the Set difference between Set *this and Set S
 * Set *this is modified and then returned
 */
Set& Set::operator-=(const Set& S) {
    Node* p_this = head->next;
    Node* p_other = S.head->next;

    while (p_this != tail && p_other != S.tail) {
        if (p_this->value == p_other->value) {
            p_this = p_this->next;
            p_other = p_other->next;
            remove_node(p_this->prev);
        }
        else if (p_this->value < p_other->value) {
            p_this = p_this->next;
        }
        else {
            p_other = p_other->next;
        }
    }
    return *this;
}


/* ******************************************** *
 * Private Member Functions -- Implementation   *
 * ******************************************** */

/*
 * Insert a new Node storing val after the Node pointed by p
 * \param p pointer to a Node
 * \param val value to be inserted  after position p
 */

void Set::insert_node(Node* p, int val) {
    if (p == nullptr || p == tail)
        return;
    //Set p->next->prev to the new node then set p->next the the same
    p->next = p->next->prev = new Node(val, p->next, p);
    counter++;
}

/*
 * Remove the Node pointed by p
 * \param p pointer to a Node
 */
void Set::remove_node(Node* p) {
    if (p == nullptr || p == head || p == tail)
        return;
    //Relink the list
    p->next->prev = p->prev;
    p->prev->next = p->next;
    //Delete p
    delete p;
    counter--;
}

/*
 * Write Set *this to stream os
 */
void Set::write_to_stream(std::ostream& os) const {
    if (is_empty()) {
        os << "Set is empty!";
    } else {
        Set::Node* ptr{head->next};

        os << "{ ";
        while (ptr != tail) {
            os << ptr->value << " ";
            ptr = ptr->next;
        }
        os << "}";
    }
}
