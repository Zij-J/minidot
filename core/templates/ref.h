#ifndef REF
#define REF

#include "../debugger/code_tester.h"

// pointer-ish, reference counting, simplied from Godot: https://github.com/godotengine/godot/blob/master/core/object/ref_counted.h
template <typename Type>
class Ref {
public:
    Ref() {} // empty reference (still has `counter` for `_unref`)

    Ref(Type *origin): counter(new int(1)), object(origin) {} // construct object from outside 

    template <typename Type_Derived>
	Ref(Type_Derived *origin): counter(new int(1)), object(origin) {} // construct object from outside (derived class)

    template <typename... VarArgs>
    Ref(VarArgs... params): counter(new int(1)), object(new Type(params...)) {} // construct object 

    Ref(Ref& origin) { // copy construct (shadow copy reference)
        _ref(origin);
    }

    ~Ref() {
        _unref();
    }


    // void operator=(const Type* origin) { // reassign (construct object from outside)
    //     if (object != nullptr) {
    //         _unref();        
    //     }

    //     counter = new int(1);
    //     object = origin.object;
    // }
    void operator=(const Ref& origin) { // reassign (copy construct, shadow copy reference)
        _ref(origin);
    }

    Type &operator*() const { // using `*` to get object
		return *object;
	}
    Type *operator->() const { // `->` to correctly get data members
		return object;
	}


private:
    int *counter = new int(1);
    Type *object = nullptr;

    void _unref() {
        --*counter;
        if (*counter == 0) {
            delete counter;
            delete object;
            DEBUG_COUT("clear");
        }
    }
    
    void _ref(Ref& origin) {
        if (object != nullptr) {
            _unref();        
        }

        counter = origin.counter;
        object = origin.object;
        ++*counter;
    }
};

#endif