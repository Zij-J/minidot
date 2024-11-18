// This is learned from: https://blog.molecular-matters.com/2011/09/19/generic-type-safe-delegates-and-events-in-c/
// Delegate in C#, demake in C++! (I can't understand all of them, so I just use what I understand): ONLY ALLOW ONE LISTENER, CAN'T HAVE ARGUMENT
// need "internal" + template, creating functions to call real listener function, for member functions/normal function difference

#ifndef DELEGATE_H
#define DELEGATE_H

#define DELEGATE_MAX 4

class Delegate
{
public:
    typedef void (*internal_function_pointer)(void *); // use (void *) to receive all type of instance

    Delegate() {
        internal_notify_function[0] = nullptr; // for `nothing` detection
    }
    ~Delegate() {}

    // make delegate know the listener(function)
    template <void (*listener)(void)>
    void add_listener() {
        if (size >= DELEGATE_MAX) {
            return ;
        }

        delegate_instance[size] = nullptr;
        internal_notify_function[size] = &_internal_notify_delegate<listener>;
        ++size;
    }
    // make delegate know the listener(member function)
    template <class C, void (C::*listener)(void)>
    void add_listener(C *listening_instacne) {
        if (size >= DELEGATE_MAX) {
            return ;
        }

        delegate_instance[size] = listening_instacne;
        internal_notify_function[size] = &_internal_notify_delegate<C, listener>; 
        ++size;
    }


    // call the listener (if have)
    void notify() {
        if (internal_notify_function[0] == nullptr) { // 90% will be nullptr, can skip lots of init
            return ;
        }

        internal_notify_function[0](delegate_instance[0]);
        for (int i = 1; i < size; ++i) {
            if (internal_notify_function[i] == nullptr) {
                return ;
            }
            internal_notify_function[i](delegate_instance[i]);
        }
    }


private:
    void *delegate_instance[DELEGATE_MAX];
    internal_function_pointer internal_notify_function[DELEGATE_MAX]; 

    int size = 0;


    template <void (*listener)(void)> // for normal function
    static void _internal_notify_delegate(void *give_me_nullptr) {
        listener();
    }
    template <class C, void (C::*listener)(void)> // for member function
    static void _internal_notify_delegate(void *listener_instance) {
        (static_cast<C *>(listener_instance)->*listener)(); // make instance pointer back to original to get the listener function
    }
};


#endif