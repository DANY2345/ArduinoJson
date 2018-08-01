// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Configuration.hpp"
#include "JsonArrayData.hpp"
#include "JsonObjectData.hpp"
#include "JsonVariant.hpp"
#include "Numbers/parseFloat.hpp"
#include "Numbers/parseInteger.hpp"

#include <string.h>  // for strcmp

namespace ArduinoJson {

inline void JsonVariant::set(const JsonArray& array) {
  if (!_data) return;
  _data->set(array._data);
}

inline void JsonVariant::set(const Internals::JsonArraySubscript& value) {
  set(value.as<JsonVariant>());
}

inline void JsonVariant::set(const JsonObject& object) {
  if (!_data) return;
  _data->set(object._data);
}

template <typename TString>
inline void JsonVariant::set(
    const Internals::JsonObjectSubscript<TString>& value) {
  set(value.as<JsonVariant>());
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonArray>::value,
    JsonArray>::type
JsonVariant::as() const {
  return _data ? JsonArray(_data->asArray()) : JsonArray();
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonObject>::value,
    T>::type
JsonVariant::as() const {
  return _data ? JsonObject(_data->asObject()) : JsonObject();
}

inline bool JsonVariant::variantIsBoolean() const {
  using namespace Internals;
  return _data && _data->type == JSON_BOOLEAN;
}

inline bool JsonVariant::variantIsInteger() const {
  using namespace Internals;

  return _data && (_data->type == JSON_POSITIVE_INTEGER ||
                   _data->type == JSON_NEGATIVE_INTEGER);
}

inline bool JsonVariant::variantIsFloat() const {
  using namespace Internals;

  return _data &&
         (_data->type == JSON_FLOAT || _data->type == JSON_POSITIVE_INTEGER ||
          _data->type == JSON_NEGATIVE_INTEGER);
}

}  // namespace ArduinoJson
