
#ifndef COMMON_INTERFACES_HPP
#define COMMON_INTERFACES_HPP


#include <string>


class ITogglable {
public:
    virtual void Toggle(bool state) {}
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
