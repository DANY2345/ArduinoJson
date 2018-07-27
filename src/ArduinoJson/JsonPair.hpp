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

// A key value pair for JsonObjectData.
class JsonPair {
 public:
  JsonPair(Internals::JsonPairData* data) : _data(data) {}
  JsonPair() : _data(0) {}

  const char* key() const {
    return _data->key;
  }

  JsonVariantRef value() const {
    return JsonVariantRef(&_data->value);
  }

 private:
  mutable Internals::JsonPairData* _data;
};
}  // namespace ArduinoJson
