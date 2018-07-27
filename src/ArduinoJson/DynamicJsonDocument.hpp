// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonArray.hpp"
#include "JsonObject.hpp"
#include "JsonVariant.hpp"
#include "Memory/DynamicJsonBuffer.hpp"

namespace ArduinoJson {

class DynamicJsonDocument {
  Internals::DynamicJsonBuffer _buffer;
  Internals::JsonVariantData _rootData;
  JsonVariant _root;

 public:
  uint8_t nestingLimit;

  DynamicJsonDocument()
      : _root(&_rootData), nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}
  DynamicJsonDocument(size_t capacity)
      : _buffer(capacity),
        _root(&_rootData),
        nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}

  template <typename T>
  bool is() const {
    return _root.is<T>();
  }

  template <typename T>
  typename Internals::JsonVariantAs<T>::type as() const {
    return _root.as<T>();
  }

  // JsonObject to<JsonObject>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonObject>::value,
                                JsonObject>::type
  to() {
    clear();
    JsonObject object(&_buffer);
    _root.set(object);
    return object;
  }

  // JsonArray to<JsonArray>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonArray>::value,
                                JsonArray>::type
  to() {
    clear();
    JsonArray array(&_buffer);
    _root.set(array);
    return array;
  }

  // JsonVariant& to<JsonVariant>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonVariant>::value,
                                T&>::type
  to() {
    clear();
    return _root;
  }

  Internals::DynamicJsonBuffer& buffer() {
    return _buffer;
  }

  void clear() {
    _buffer.clear();
    _root = JsonVariant();
  }

  size_t memoryUsage() const {
    return _buffer.size();
  }

  template <typename Visitor>
  void visit(Visitor& visitor) const {
    return _root.visit(visitor);
  }
};
}  // namespace ArduinoJson
