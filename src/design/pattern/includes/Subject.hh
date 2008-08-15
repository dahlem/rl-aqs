// Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef SUBJECT_HH
#define SUBJECT_HH


#include <vector>

#include <boost/shared_ptr.hpp>

#include "Observer.hh"



namespace des
{
    namespace design
    {



/**
 * This is is a template to declare the subject of the GOF's observer pattern.
 * Usage:
 * <code>
 *   class ConcreteSubject: public Subject<ConcreteSubject> {}
 * </code>
 * and implement a state change method that calls <code>notify()</code>. Also,
 * implement a method that allows the concrete observer to ask for the specific
 * state change.
 * This template was mainly taken from
 * <a href="http://www.codeproject.com/cpp/observer_with_templates.asp" />.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
template <class T>
class Subject
{
public:
    Subject()
        {}

    virtual ~Subject()
        {}

    /**
     * Attach an observer to a subject.
     *
     * @param Observer<T> the observer to be attached to the subject
     */
    void attach(boost::shared_ptr <design::Observer <T> > observer)
        {
            m_observers.push_back(&observer);
        }

    /**
     * Notify the attached observers.
     */
    void notify()
        {
            typename std::vector <boost::shared_ptr <Observer<T> > >::iterator it;

            for (it = m_observers.begin(); it != m_observers.end(); it++) {
                (*it)->update(boost::shared_ptr <T> (static_cast <T *>(this)));
            }
        }

private:
    std::vector <boost::shared_ptr <design::Observer <T> > > m_observers;
};

    }
}

#endif
