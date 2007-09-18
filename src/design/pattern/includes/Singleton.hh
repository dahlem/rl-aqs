#ifndef SINGLETON_HH
#define SINGLETON_HH



template <class T>
class Singleton
{
public:
    static T& getInstance() {
        static T m_instance;
        return m_instance;
    }

private:
    Singleton();
    Singleton(Singleton const&);
    ~Singleton();
};



#endif
