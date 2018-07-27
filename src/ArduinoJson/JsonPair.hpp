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
  JsonPair(Internals::JsonPairData* data)
      : _key(data->key), _value(&data->value) {}
  JsonPair() : _key(0) {}

  const char* key() const {
    return _key;
  }

  JsonVariantRef value() const {
    return _value;
  }

 private:
  const char* _key;
  JsonVariantRef _value;
};
}  // namespace ArduinoJson
