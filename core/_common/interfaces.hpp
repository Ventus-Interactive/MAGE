
#ifndef COMMON_INTERFACES_HPP
#define COMMON_INTERFACES_HPP


#include <string>


class IPrintable {
public:
    virtual const char* ToString() = 0;
};

class ICastable/*Pointer*/ {
public:
    template <class T>
    T* As() { return (T*)this; }
};

class IDestroyable {
public:
    virtual void Init()    {}
    virtual void Destroy() {}
};

class IResource : public ICastable, public IDestroyable {
public:
    //void Destroy() {}
};

class ITogglable {
public:
    virtual void ToggleState() {}
    virtual void OnEnable()  {}
    virtual void OnDisable() {}
};

class IUpdatable {
public:
    virtual void Update() {}
};

template <class T>
class ISerializable {
public:
    virtual std::string Serialize() { return ""; }
    virtual void Deserialize(T data_copy) {}
};



#endif // !COMMON_INTERFACES_HPP
