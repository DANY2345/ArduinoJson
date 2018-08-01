// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariant.hpp"
#include "Memory/StaticJsonBuffer.hpp"

namespace ArduinoJson {

template <size_t CAPACITY>
class StaticJsonDocument {
  mutable Internals::StaticJsonBuffer<CAPACITY> _buffer;
  mutable Internals::JsonVariantData _rootData;

 public:
  uint8_t nestingLimit;

  StaticJsonDocument() : nestingLimit(ARDUINOJSON_DEFAULT_NESTING_LIMIT) {}

  Internals::StaticJsonBufferBase& buffer() {
    return _buffer;
  }

  template <typename T>
  bool is() const {
    return JsonVariant(&_buffer, &_rootData).is<T>();
  }

  template <typename T>
  typename Internals::JsonVariantAs<T>::type as() const {
    return JsonVariant(&_buffer, &_rootData).as<T>();
  }

  // JsonObject to<JsonObject>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonObject>::value,
                                JsonObject>::type
  to() {
    clear();
    JsonObject object(&_buffer);
    as<JsonVariant>().set(object);
    return object;
  }

  // JsonArray to<JsonArray>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonArray>::value,
                                JsonArray>::type
  to() {
    clear();
    JsonArray array(&_buffer);
    as<JsonVariant>().set(array);
    return array;
  }

  // JsonVariant to<JsonVariant>()
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonVariant>::value,
                                JsonVariant>::type
  to() {
    clear();
    return JsonVariant(&_buffer, &_rootData);
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
