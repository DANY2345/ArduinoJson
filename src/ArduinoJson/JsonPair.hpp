// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariant.hpp"
#include "JsonVariantRef.hpp"

namespace ArduinoJson {

namespace Internals {

struct JsonPairData {
  const char* key;
  JsonVariantData value;
};
}  // namespace Internals

class JsonKey {
 public:
  explicit JsonKey(Internals::JsonPairData* data) : _data(data) {}

  operator const char*() const {
    return _data->key;
  }

  void set(const char* key) {
    _data->key = key;
  }

 private:
  Internals::JsonPairData* _data;
};

// A key value pair for JsonObjectData.
class JsonPair {
 public:
  JsonKey key() {
    return JsonKey(&_data);
  }

  const char* key() const {
    return _data.key;
  }

  JsonVariantRef value() {
    return JsonVariantRef(&_data.value);
  }

  JsonVariantRef value() const {
    return JsonVariantRef(&_data.value);
  }

 private:
  mutable Internals::JsonPairData _data;
};
}  // namespace ArduinoJson
