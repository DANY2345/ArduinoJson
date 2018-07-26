// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "JsonVariant.hpp"

namespace ArduinoJson {

struct JsonPairData {
  const char* key;
  JsonVariant value;
};

class JsonKey {
 public:
  explicit JsonKey(JsonPairData* data) : _data(data) {}

  operator const char*() const {
    return _data->key;
  }

  JsonKey& operator=(const char* key) {
    _data->key = key;
    return *this;
  }

  void set(const char* key) {
    _data->key = key;
  }

 private:
  JsonPairData* _data;
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

  JsonVariant& value() {
    return _data.value;
  }

  const JsonVariant& value() const {
    return _data.value;
  }

 private:
  JsonPairData _data;
};
}  // namespace ArduinoJson
