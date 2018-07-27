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
  mutable Internals::JsonVariantData _rootData;

 public:
  uint8_t nestingLimit;

  DynamicJsonDocument() : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}
  DynamicJsonDocument(size_t capacity)
      : _buffer(capacity), nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}

  template <typename T>
  bool is() const {
    return JsonVariant(&_rootData).is<T>();
  }

  template <typename T>
  typename Internals::JsonVariantAs<T>::type as() const {
    return JsonVariant(&_rootData).as<T>();
  }

  // JsonObject to<JsonObject>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonObject>::value,
                                JsonObject>::type
  to() {
    clear();
    JsonObject object(&_buffer);
    JsonVariant(&_rootData).set(object);
    return object;
  }

  // JsonArray to<JsonArray>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonArray>::value,
                                JsonArray>::type
  to() {
    clear();
    JsonArray array(&_buffer);
    JsonVariant(&_rootData).set(array);
    return array;
  }

  // JsonVariant& to<JsonVariant>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonVariant>::value,
                                JsonVariant>::type
  to() {
    clear();
    return JsonVariant(&_rootData);
  }

  Internals::DynamicJsonBuffer& buffer() {
    return _buffer;
  }

  void clear() {
    _buffer.clear();
    _rootData.type = Internals::JSON_UNDEFINED;
  }

  size_t memoryUsage() const {
    return _buffer.size();
  }

  template <typename Visitor>
  void visit(Visitor& visitor) const {
    return JsonVariant(&_rootData).visit(visitor);
  }
};
}  // namespace ArduinoJson
