// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data\ListIterator.hpp"
#include "JsonVariant.hpp"

namespace ArduinoJson {

// A read-write forward iterator for JsonArray
class JsonArrayIterator {
  typedef Internals::ListIterator<Internals::JsonVariantData> internal_iterator;

 public:
  JsonArrayIterator() {}
  explicit JsonArrayIterator(internal_iterator iterator)
      : _iterator(iterator) {}

  JsonVariant &operator*() const {
    _variant = JsonVariant(&*_iterator);
    return _variant;
  }
  JsonVariant *operator->() {
    _variant = JsonVariant(&*_iterator);
    return &_variant;
  }

  bool operator==(const JsonArrayIterator &other) const {
    return _iterator == other._iterator;
  }

  bool operator!=(const JsonArrayIterator &other) const {
    return _iterator != other._iterator;
  }

  JsonArrayIterator &operator++() {
    ++_iterator;
    return *this;
  }

  JsonArrayIterator &operator+=(size_t distance) {
    _iterator += distance;
    return *this;
  }

  internal_iterator internal() {
    return _iterator;
  }

 private:
  internal_iterator _iterator;
  mutable JsonVariant _variant;
};
}  // namespace ArduinoJson
