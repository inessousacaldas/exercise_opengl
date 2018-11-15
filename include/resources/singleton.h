#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton 
{
public:
    // Singleton instance
    static T& Instance() 
    {
        static T instance;
        return instance;
    }
    // Copy constructor
    Singleton(const Singleton&) = delete;
    // Move constructor
    Singleton(Singleton&&) = delete;
    // Copy assignment
    Singleton& operator =(const Singleton&) = delete;
    // Move assignment
    Singleton& operator =(Singleton&&) = delete;
protected:
    Singleton() {};
    virtual ~Singleton() {};
};

#endif