// Copyright (C) 2007 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//  
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without 
// modifications, as long as this notice is preserved.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef SINGLETON_HH
#define SINGLETON_HH


namespace des
{
    namespace design
    {
        

/**
 * This is is a template to declare Scott Meyer's singleton pattern.
 * Usage: <code>typedef Singleton<AClass> ASingleton;</code>
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
template <class T>
class Singleton
{
public:
    /**
     * get the only instance of this class.
     *
     * @return an instance of the singleton class
     */
    static T& getInstance() {
        static T m_instance;
        return m_instance;
    }

private:
    /**
     * Private constructor, because this class is its own resource manager.
     */
    Singleton();

    /**
     * Private copy constructor, because this class is its own resource
     * manager.
     */
    Singleton(Singleton const&);

    /**
     * Private destructor, because this class is its own resource manager.
     */
    ~Singleton();
};


    }
}

#endif
