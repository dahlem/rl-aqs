// Copyright (C) 2007-2009 Dominik Dahlem <Dominik.Dahlem@gmail.com>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __DES_DESIGN_SINGLETON_HH__
#define __DES_DESIGN_SINGLETON_HH__

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_OPENMP
# include <omp.h>
#endif /* HAVE_OPENMP */


namespace des
{
    namespace design
    {


/**
 * This is is a template to declare Scott Meyer's singleton pattern.
 * Usage: <code>typedef Singleton<AClass> ASingleton;</code>
 *
 * @author <a href="mailto:Dominik.Dahlem@gmail.com">Dominik Dahlem</a>
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
//         static T* cache = NULL;

// #ifdef HAVE_OPENMP
// # pragma omp threadprivate (cache)
// #endif /* HAVE_OPENMP */
//         if (cache == NULL) {
// #ifdef HAVE_OPENMP
// # pragma omp critical (DES_SINGLETON_INSTANCE)
// #endif /* HAVE_OPENMP */
//             if (cache == NULL) {
//                 cache = new T;
//             }
//         }

//         return *cache;
        static T m_instance;
        return m_instance;
    }

    /**
     * Private constructor, because this class is its own resource manager.
     */
    Singleton()
        {}

    /**
     * Private destructor, because this class is its own resource manager.
     */
    virtual ~Singleton()
        {}

private:
    Singleton(Singleton const&); // Shouldn't be used
    Singleton& operator=(Singleton const&); // Shouldn't be used;
};

    }
}





#endif /* __DES_DESIGN_SINGLETON_HH__ */
