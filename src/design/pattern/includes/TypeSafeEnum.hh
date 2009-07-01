// Copyright (C) 2009 Dominik Dahlem <Dominik.Dahlem@cs.tcd.ie>
//
// This file is free software; as a special exception the author gives
// unlimited permission to copy and/or distribute it, with or without
// modifications, as long as this notice is preserved.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

#ifndef TYPESAFEENUM_HH
#define TYPESAFEENUM_HH


namespace des
{
    namespace design
    {

    template<typename T, int N>
    struct enumerator {
        const static int value = N;
        typedef enumerator<T, N + 1> next;
    };


    }
}

#endif

