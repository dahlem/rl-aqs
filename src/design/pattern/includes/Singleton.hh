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
