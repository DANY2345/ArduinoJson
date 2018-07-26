// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Configuration.hpp"
#include "JsonArrayData.hpp"
#include "JsonObjectData.hpp"
#include "JsonVariantRef.hpp"

#include <string.h>  // for strcmp

namespace ArduinoJson {

inline void JsonVariantRef::set(JsonArray array) {
  if (!array.isNull()) {
    _data->type = Internals::JSON_ARRAY;
    _data->content.asArray = array._data;
  } else {
    _data->type = Internals::JSON_UNDEFINED;
  }
}

inline void JsonVariantRef::set(const Internals::JsonArraySubscript& value) {
  set(value.as<JsonVariantRef>());
}

inline void JsonVariantRef::set(JsonObject object) {
  if (!object.isNull()) {
    _data->type = Internals::JSON_OBJECT;
    _data->content.asObject = object._data;
  } else {
    _data->type = Internals::JSON_UNDEFINED;
  }
}

template <typename TString>
inline void JsonVariantRef::set(
    const Internals::JsonObjectSubscript<TString>& value) {
  set(value.as<JsonVariant>());
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonArray>::value,
    JsonArray>::type
JsonVariantRef::as() const {
  return variantAsArray();
}

template <typename T>
inline typename Internals::enable_if<
    Internals::is_same<typename Internals::remove_const<T>::type,
                       JsonObject>::value,
    T>::type
JsonVariantRef::as() const {
  return variantAsObject();
}

inline JsonArray JsonVariantRef::variantAsArray() const {
  if (_data->type == Internals::JSON_ARRAY) return _data->content.asArray;
  return JsonArray();
}

inline JsonObject JsonVariantRef::variantAsObject() const {
  if (_data->type == Internals::JSON_OBJECT) return _data->content.asObject;
  return JsonObject();
}

template <typename T>
inline T JsonVariantRef::variantAsInteger() const {
  using namespace Internals;
  switch (_data->type) {
    case JSON_UNDEFINED:
    case JSON_UNPARSED:
      return 0;
    case JSON_POSITIVE_INTEGER:
    case JSON_BOOLEAN:
      return T(_data->content.asInteger);
    case JSON_NEGATIVE_INTEGER:
      return T(~_data->content.asInteger + 1);
    case JSON_STRING:
      return parseInteger<T>(_data->content.asString);
    default:
      return T(_data->content.asFloat);
  }
}

inline const char* JsonVariantRef::variantAsString() const {
  using namespace Internals;
  return _data->type == JSON_STRING ? _data->content.asString : NULL;
}

template <typename T>
inline T JsonVariantRef::variantAsFloat() const {
  using namespace Internals;
  switch (_data->type) {
    case JSON_UNDEFINED:
    case JSON_UNPARSED:
      return 0;
    case JSON_POSITIVE_INTEGER:
    case JSON_BOOLEAN:
      return static_cast<T>(_data->content.asInteger);
    case JSON_NEGATIVE_INTEGER:
      return -static_cast<T>(_data->content.asInteger);
    case JSON_STRING:
      return parseFloat<T>(_data->content.asString);
    default:
      return static_cast<T>(_data->content.asFloat);
  }
}

inline bool JsonVariantRef::variantIsBoolean() const {
  using namespace Internals;
  return _data->type == JSON_BOOLEAN;
}

inline bool JsonVariantRef::variantIsInteger() const {
  using namespace Internals;

  return _data->type == JSON_POSITIVE_INTEGER ||
         _data->type == JSON_NEGATIVE_INTEGER;
}

inline bool JsonVariantRef::variantIsFloat() const {
  using namespace Internals;

  return _data->type == JSON_FLOAT || _data->type == JSON_POSITIVE_INTEGER ||
         _data->type == JSON_NEGATIVE_INTEGER;
}

}  // namespace ArduinoJson
