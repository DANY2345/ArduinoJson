// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data\ListIterator.hpp"
#include "JsonPair.hpp"

namespace ArduinoJson {

// A read-write forward iterator for JsonArray
class JsonObjectIterator {
  typedef Internals::ListIterator<Internals::JsonPairData> internal_iterator;

 public:
  JsonObjectIterator() {}
  explicit JsonObjectIterator(internal_iterator iterator)
      : _iterator(iterator) {}

  JsonPair &operator*() const {
    _pair = JsonPair(&*_iterator);
    return _pair;
  }
  JsonPair *operator->() {
    _pair = JsonPair(&*_iterator);
    return &_pair;
  }

  bool operator==(const JsonObjectIterator &other) const {
    return _iterator == other._iterator;
  }

  bool operator!=(const JsonObjectIterator &other) const {
    return _iterator != other._iterator;
  }

  JsonObjectIterator &operator++() {
    ++_iterator;
    return *this;
  }

  JsonObjectIterator &operator+=(size_t distance) {
    _iterator += distance;
    return *this;
  }

  internal_iterator internal() {
    return _iterator;
  }

 private:
  internal_iterator _iterator;
  mutable JsonPair _pair;
};
}  // namespace ArduinoJson
