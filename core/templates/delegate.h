// This is learned from: https://blog.molecular-matters.com/2011/09/19/generic-type-safe-delegates-and-events-in-c/
// Delegate in C#, demake in C++! (I can't understand all of them, so I just use what I understand): ONLY ALLOW ONE LISTENER, CAN'T HAVE ARGUMENT
// need "internal" + template, creating functions to call real listener function, for member functions/normal function difference

#ifndef DELEGATE_H
#define DELEGATE_H


class Delegate
{
public:
    typedef void (*internal_function_pointer)(void *); // use (void *) to receive all type of instance

    Delegate() {}
    ~Delegate() {}

    // make delegate know the listener(function)
    template <void (*listener)(void)>
    void add_listener() {
        internal_notify_function = &internal_notify_delegate<listener>;
    }
    // make delegate know the listener(member function)
    template <class C, void (C::*listener)(void)>
    void add_listener(C *listening_instacne) {
        delegate_instance = listening_instacne;
        internal_notify_function = &internal_notify_delegate<C, listener>; 
    }


    // call the listener (if have)
    void notify() {
        if (internal_notify_function != nullptr)
            internal_notify_function(delegate_instance);
    }


private:
    void *delegate_instance = nullptr;
    internal_function_pointer internal_notify_function = nullptr; 


    template <void (*listener)(void)> // for normal function
    static void internal_notify_delegate(void *give_me_nullptr) {
        listener();
    }
    template <class C, void (C::*listener)(void)> // for member function
    static void internal_notify_delegate(void *listener_instance) {
        (static_cast<C *>(listener_instance)->*listener)(); // make instance pointer back to original to get the listener function
    }
};


#endif