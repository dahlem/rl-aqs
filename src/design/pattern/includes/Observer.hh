// Copyright (C) 2007, 2008 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef OBSERVER_HH
#define OBSERVER_HH

#include <boost/shared_ptr.hpp>


namespace des
{
    namespace design
    {

/**
 * This is is a template to declare the observer of the GOF's observer pattern.
 * Usage:
 * <code>
 *   class ConcreteObserver : public Observer<ConcreteSubject> {}
 * </code>
 * and implement the <code>update()</code> method in order to act upon the state
 * change.
 *
 * This template was mainly taken from
 * <a href="http://www.codeproject.com/cpp/observer_with_templates.asp" />.
 *
 * @author <a href="mailto:Dominik.Dahlem@cs.tcd.ie">Dominik Dahlem</a>
 */
template <class T>
class Observer
{
public:
    Observer()
        {}

    virtual ~Observer()
        {}

    /**
     * Update the internal state of the observer based on the state change of
     * the subject. The subject is passed as a parameter into this method in
     * order to be able to call back on the specific change.
     *
     * @param T* the subject that notified the concrete observer.
     */
    virtual void update(boost::shared_ptr<T> subject) = 0;
};


    }
}

#endif
