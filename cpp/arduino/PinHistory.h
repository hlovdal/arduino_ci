#pragma once
#include "queue.h"
#include "WString.h"

// pins with history.
template <typename T>
class PinHistory {
private:
  queue<T> qIn;
  queue<T> qOut;

  unsigned long mLastRead = 0;

  void clear(queue<T>* q) {
    while (!q->empty()) q->pop();
  }

  void clear() {
    clear(&qOut);
    clear(&qIn);
  }

public:
  PinHistory() {}

  void reset(T val) {
    clear();
    qOut.push(val);
  }

  unsigned int historySize() { return qOut.size(); }

  // This returns the "value" of the pin in a raw sense
  operator T() const {
    if (mLastRead == 0 && qIn.size()) return qIn.front();
    return qOut.back();
  }

  // this sets the value of the pin authoritatively
  // so if there was a queue, dump it.
  // the actual "set" operation doesn't happen until the next read
  const T &operator=(const T& i) {
    qOut.push(i);
    return qOut.back();
  }

  // This returns the "value" of the pin according to the queued values
  // if now is the same as the last read time, return the same val and exit
  // if there is input, advance it to the output.
  // then take the latest output.
  T retrieve() {
    if (qIn.size()) {
      qOut.push(qIn.front());
      qIn.pop();
    }
    return qOut.back();
  }

  // enqueue a set of elements
  void fromArray(T* arr, unsigned int length) {
    for (int i = 0; i < length; ++i) qIn.push(arr[i]);
  }

  // enqueue ascii bits
  void fromAscii(String input, bool bigEndian) {
    String ret = "";

    // 8 chars at a time, form up
    for (int j = 0; j < input.length(); ++j) {
      for (int i = 0; i < 8; ++i) {
        int shift = bigEndian ? 7 - i : i;
        unsigned char mask = (0x01 << shift);
        qIn.push(mask & input[j]);
      }
    }
  }

  // copy elements to an array, up to a given length
  // return the number of elements moved
  int toArray(T* arr, unsigned int length) {
    queue<T> q2(qOut);

    int ret = 0;
    for (int i = 0; i < length && q2.size(); ++i) {
      arr[i] = q2.front();
      q2.pop();
      ++ret;
    }
    return ret;
  }

  // see if the array matches the elements in the queue
  bool hasElements(T* arr, unsigned int length) {
    int i;
    queue<T> q2(qOut);
    for (i = 0; i < length && q2.size(); ++i) {
      if (q2.front() != arr[i]) return false;
      q2.pop();
    }
    return i == length;
  }

  // convert the pin history to a string as if it was Serial comms
  // start from offset, consider endianness
  String toAscii(unsigned int offset, bool bigEndian) {
    String ret = "";

    queue<T> q2(qOut);

    while (offset) {
      q2.pop();
      --offset;
    }

    if (offset) return ret;

    // 8 chars at a time, form up
    while (q2.size() >= 8) {
      unsigned char acc = 0x00;
      for (int i = 0; i < 8; ++i) {
        int shift = bigEndian ? 7 - i : i;
        T val = q2.front();
        unsigned char bit = val ? 0x1 : 0x0;
        acc |= (bit << shift);
        q2.pop();
      }
      ret.append(String((char)acc));
    }

    return ret;
  }
};

