#ifndef REF
#define REF


// pointer-ish, unprotected (directly `static_cast`) polymorphism reference counting, simplied from Godot: https://github.com/godotengine/godot/blob/master/core/object/ref_counted.h
template <typename Type>
class Ref {
    template <typename Type_Other> // base/derived Ref can access `object` pointer 
    friend class Ref;

public:
    Ref() {} // empty reference (still has `counter` for `_unref`)

    Ref(Type *origin): counter(new int(1)), object(origin) {} // construct object from outside 

    template <typename Type_Derived>
	Ref(Type_Derived *origin): counter(new int(1)), object(origin) {} // construct object from outside (derived class only)

    template <typename... VarArgs>
    Ref(VarArgs... params): counter(new int(1)), object(new Type(params...)) {} // construct object 

    template <typename Type_Other>    
    Ref(Ref<Type_Other>& origin) { // copy construct (shadow copy reference)
        _ref<Type_Other>(origin);
    }
    Ref(Ref& origin) { // copy construct (shadow copy reference)
        _ref(origin);
    }

    ~Ref() {
        _unref();
    }

    // template <typename Type_Derived>
    // operator Type_Derived *() const {  // pointer typecast operator overload (if need `static_cast<Type_Derived *>`)
    //     return static_cast<Type_Derived *>(object); 
    // }

    // void operator=(const Type* origin) { // reassign (construct object from outside)
    //     if (object != nullptr) {
    //         _unref();        
    //     }

    //     counter = new int(1);
    //     object = origin.object;
    // }
    template <typename Type_Other>
    void operator=(const Ref<Type_Other>& origin) { // reassign (copy construct, shadow copy reference)
        _ref<Type_Other>(origin);
    }
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
        }
    }
    
    template <typename Type_Other>
    void _ref(const Ref<Type_Other>& origin) {
        if (object != nullptr) {
            _unref();        
        }

        counter = origin.counter;
        object = static_cast<Type *>(origin.object);
        ++*counter;
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