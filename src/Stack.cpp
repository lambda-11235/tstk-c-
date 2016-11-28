

template<class T>
Stack<T>::Stack() {}


template<class T>
T Stack<T>::pop() {
  if(size() > 0) {
    T ret = stk.back();
    stk.pop_back();
    return ret;
  } else {
    throw StackUnderflow();
  }
}


template<class T>
T Stack<T>::top() const {
  if(size() > 0) {
    return stk.back();
  } else {
    throw StackUnderflow();
  }
}


template<class T>
T Stack<T>::get(int index) const {
  if(index < size()) {
    return stk[size() - index - 1];
  } else {
    throw StackUnderflow();
  }
}


template<class T>
void Stack<T>::set(int index, const T& elem) {
  if(index < size()) {
    stk[size() - index - 1] = elem;
  } else {
    throw StackUnderflow();
  }
}
