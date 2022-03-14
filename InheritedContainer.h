#include "Forward.h"
/*
Lets say you're making like, a programming language, perhaps
and you need to remember all the various things that a particular class has as members, either from its own definition or an inherited one
like, /fim/bam/bino needs to remember the shit it gets from /fim, /fim/bam, and /fim/bam/bino

If each element caches its properties, there is a lot of wasted time and memory put into initializing all that redundant memory.
This provides an alternative, allowing for a particularly meta "doubly-linked list" which allows an ascending iteration across the tree.

Written generically in case someone might want to use this for something that isn't std::unordered_map<> linking.
*/
template<typename Container>
class InheritedContainer
{
    Container my_container;
    InheritedContainer* parent;
    InheritedContainer* child;

    class Iterator
    {
        typedef decltype(std::begin(std::declval<Container&>())) sub_iterator;
        sub_iterator it;
        bool is_at_end; // Can't *precisely* use std::end(cont) here, since we may try alternate containers
        Container* current_container;
        Container* const master_container;
        InheritedContainer* current_parent;
    public:
        Iterator(Container& cont, InheritedContainer* p)
            :it(std::begin(cont))
            ,current_container(&cont)
            ,master_container(&cont)
            ,current_parent(p)
            ,is_at_end(false)
        {

        }
        Iterator(Container& cont)
            :master_container(&cont)
            ,is_at_end(true)
        {
            
        }
        bool operator==(const Iterator& other) const
        {
            if(other.is_at_end)
            {
                return is_at_end; // All end()s return as equal. For better or worse.
            }
            if(current_container == other.current_container)
                return it == other.it;
            return false;

        }
        //bool operator!=(const Iterator& other) const { return !(this->operator==(other));}

        auto operator*() // Most mysterious "auto" I've ever used
        {
            return it.operator*();
        }
        Iterator& operator++(int)
        {
            if(is_at_end) [[unlikely]]
                return *this;
            it++;
            if(it == std::end(*current_container)) // If we're at the end of this container
            {
                if(!current_parent) // and we can go no higher up
                {
                    is_at_end = true;
                    return *this; // We are now end().
                }
                //Otherwise, go up and start iterating from the next thing in the chain.
                current_container = &(current_parent->container());
                it = std::begin(*current_container);
                current_parent = current_parent->parent;
                return *this;
            }
            return *this;
        }
        Iterator& operator++() { return this->operator++(0);}
    };

    //These awkward, private versions of set_parent/child exist to break the recursion of the parent and child updating each other.
    void new_child_event(InheritedContainer* new_son)
    {
#ifdef DEBUG
        if(child) [[unlikely]]
        {
            std::cout << "WARNING: Overwrote existing child relationship of InheritedContainer!\n";
        }
#endif
        child = new_son;
    }
    void new_parent_event(InheritedContainer* new_dad)
    {
#ifdef DEBUG
        if(parent) [[unlikely]]
        {
            std::cout << "WARNING: Overwrote existing child relationship of InheritedContainer!\n";
        }
#endif
        parent = new_dad;
    }
public:
    Container& container() { return my_container; }
    Iterator begin() { return Iterator(my_container,parent);}
    const Iterator end() { return Iterator(my_container);}
    void set_parent(InheritedContainer& new_dad)
    {
        parent = &new_dad;
        new_dad.new_child_event(this);
    }
    void set_child(InheritedContainer& new_child)
    {
        child = &new_child;
        new_child.new_parent_event(this);
    }
    

    InheritedContainer(Container&& cont, InheritedContainer& daddy)
    :my_container(cont)
    ,parent(&daddy)
    ,child(nullptr)
    {
    }
    InheritedContainer(Container&& cont)
    :my_container(cont)
    ,parent(nullptr)
    ,child(nullptr)
    {
    }
    InheritedContainer()
    :my_container(Container())
    ,parent(nullptr)
    ,child(nullptr)
    {
    }
};