#ifndef REF
#define REF


// pointer-ish, reference counting, simplied from Godot: https://github.com/godotengine/godot/blob/master/core/object/ref_counted.h
template <typename Type>
class Ref {
    template <typename Type_Other> // base/derived Ref can access `object` pointer 
    friend class Ref;

public:
    Ref() {} // empty reference (still has `counter` for `_unref`)

    Ref(Type *origin): counter(new int(1)), object(origin) {} // construct object from outside 

    template <typename Type_Derived>
	Ref(Type_Derived *origin): counter(new int(1)), object(origin) {} // construct object from outside (derived class)

    template <typename... VarArgs>
    Ref(VarArgs... params): counter(new int(1)), object(new Type(params...)) {} // construct object 

    template <typename Type_Based>
    Ref(Ref<Type_Based>& origin) { // copy construct (shadow copy reference)
        _ref<Type_Based>(origin);
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
    template <typename Type_Base>
    void operator=(const Ref<Type_Base>& origin) { // reassign (copy construct, shadow copy reference)
        _ref<Type_Base>(origin);
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
    
    template <typename Type_Base>
    void _ref(const Ref<Type_Base>& origin) {
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