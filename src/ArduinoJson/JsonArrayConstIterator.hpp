// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/ListConstIterator.hpp"
#include "JsonVariant.hpp"

namespace ArduinoJson {

// A read-write forward iterator for JsonArray
class JsonArrayConstIterator {
  typedef Internals::ListConstIterator<Internals::JsonVariantData>
      internal_iterator;

 public:
  JsonArrayConstIterator() {}
  explicit JsonArrayConstIterator(internal_iterator iterator)
      : _iterator(iterator) {}

  const JsonVariant &operator*() const {
    _variant = JsonVariant(*_iterator);
    return _variant;
  }
  const JsonVariant *operator->() {
    _variant = JsonVariant(*_iterator);
    return &_variant;
  }

  bool operator==(const JsonArrayConstIterator &other) const {
    return _iterator == other._iterator;
  }

  bool operator!=(const JsonArrayConstIterator &other) const {
    return _iterator != other._iterator;
  }

  JsonArrayConstIterator &operator++() {
    ++_iterator;
    return *this;
  }

  JsonArrayConstIterator &operator+=(size_t distance) {
    _iterator += distance;
    return *this;
  }

 private:
  internal_iterator _iterator;
  mutable JsonVariant _variant;
};
}  // namespace ArduinoJson
