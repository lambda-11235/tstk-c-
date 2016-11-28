
#include <ostream>
#include <vector>

#include "exception/stack.h"


#ifndef STACK_H
#define STACK_H

/**
 * A stack that conforms to TSTK semantics.
 */
template<class T>
class Stack {
  std::vector<T> stk;

public:
  Stack();

  T pop();

  inline void push(const T& elem) { stk.push_back(elem); }

  T top() const;

  inline int size() const { return stk.size(); }

  inline void clear() { stk.clear(); }

  /**
   * Get an element that is indexed starting at the top of the stack.
   */
  T get(int index) const;

  /**
   * Set an element's value, where indexing starts at the top of the stack.
   */
  void set(int index, const T& elem);

  friend std::ostream& operator<<(std::ostream& ostrm, const Stack<T>& stack) {
    ostrm << '[';

    for(int i = 0; i < stack.size(); i++) {
      ostrm << stack.stk[i];

      if(i != stack.size() - 1) {
        ostrm << ", ";
      }
    }

    ostrm << ']';

    return ostrm;
  }
};

#include "Stack.cpp"

#endif /* end of include guard: STACK_H */
