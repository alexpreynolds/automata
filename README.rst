Automata
--------

*Automata* is a small library for generating one-dimensional, two-color cellular automata. It aims to be fast, general and portable, relying only on the C++ Standard Template Library (STL). Future revisions may add support for totalistic CA.

Sample usage:

```
#include "Automata.h"
#include <new>
...
try {
        Automata *automaton = new Automata(30, false, 0);
        automaton->fillBuffer();
        automaton->printBuffer();
        delete automaton;
}
catch (std::bad_alloc &exc) {
        cout << exc.what() << endl;
}
```
