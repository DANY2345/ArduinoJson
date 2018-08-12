// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Numbers/parseFloat.hpp"
#include "../Numbers/parseInteger.hpp"
#include "JsonVariantContent.hpp"
#include "JsonVariantType.hpp"

namespace ArduinoJson {
namespace Internals {

struct JsonVariantData {
  JsonVariantType type;
  JsonVariantContent content;

  void setBoolean(bool value) {
    type = JSON_BOOLEAN;
    content.asInteger = static_cast<JsonUInt>(value);
  }

  void setFloat(JsonFloat value) {
    type = JSON_FLOAT;
    content.asFloat = value;
  }

  void setInteger(JsonInteger value) {
    if (value > 0)
      setPostiveInteger(value);
    else
      setNegativeInteger(-value);
  }

  void setNegativeInteger(JsonUInt value) {
    type = JSON_NEGATIVE_INTEGER;
    content.asInteger = value;
  }

  void setPostiveInteger(JsonUInt value) {
    type = JSON_POSITIVE_INTEGER;
    content.asInteger = value;
  }

  // set(const char*);
  // set(const signed char*);
  // set(const unsigned char*);
  void setString(const char *value) {
    type = JSON_STRING;
    content.asString = value;
  }

  // Create a JsonVariant containing an unparsed string
  void setRaw(const char *data, size_t size) {
    type = JSON_UNPARSED;
    content.asRaw.data = data;
    content.asRaw.size = size;
  }

  void setUndefined() {
    type = JSON_UNDEFINED;
  }

  void setArray(JsonArrayData &array) {
    type = JSON_ARRAY;
    content.asArray = &array;
  }

  void setObject(JsonObjectData &object) {
    type = JSON_OBJECT;
    content.asObject = &object;
  }

  JsonArrayData *asArray() const {
    return type == JSON_ARRAY ? content.asArray : 0;
  }

  JsonObjectData *asObject() const {
    return type == JSON_OBJECT ? content.asObject : 0;
  }

  template <typename T>
  T asInteger() const {
    switch (type) {
      case JSON_UNDEFINED:
      case JSON_UNPARSED:
        return 0;
      case JSON_POSITIVE_INTEGER:
      case JSON_BOOLEAN:
        return T(content.asInteger);
      case JSON_NEGATIVE_INTEGER:
        return T(~content.asInteger + 1);
      case JSON_STRING:
        return parseInteger<T>(content.asString);
      default:
        return T(content.asFloat);
    }
  }

  template <typename T>
  T asFloat() const {
    switch (type) {
      case JSON_UNDEFINED:
      case JSON_UNPARSED:
        return 0;
      case JSON_POSITIVE_INTEGER:
      case JSON_BOOLEAN:
        return static_cast<T>(content.asInteger);
      case JSON_NEGATIVE_INTEGER:
        return -static_cast<T>(content.asInteger);
      case JSON_STRING:
        return parseFloat<T>(content.asString);
      default:
        return static_cast<T>(content.asFloat);
    }
  }

  const char *asString() const {
    return type == JSON_STRING ? content.asString : NULL;
  }

  bool isBoolean() const {
    return type == JSON_BOOLEAN;
  }

  bool isInteger() const {
    return type == JSON_POSITIVE_INTEGER || type == JSON_NEGATIVE_INTEGER;
  }

  bool isFloat() const {
    return type == JSON_FLOAT || type == JSON_POSITIVE_INTEGER ||
           type == JSON_NEGATIVE_INTEGER;
  }

  bool isArray() const {
    return type == Internals::JSON_ARRAY;
  }
  bool isObject() const {
    return type == Internals::JSON_OBJECT;
  }
  bool isString() const {
    return type == Internals::JSON_STRING;
  }

  template <typename Visitor>
  void visit(Visitor &visitor) const {
    switch (type) {
      case JSON_FLOAT:
        return visitor.acceptFloat(content.asFloat);

      case JSON_ARRAY:
        return visitor.acceptArray(*content.asArray);

      case JSON_OBJECT:
        return visitor.acceptObject(*content.asObject);

      case JSON_STRING:
        return visitor.acceptString(content.asString);

      case JSON_UNPARSED:
        return visitor.acceptRawJson(content.asRaw.data, content.asRaw.size);

      case JSON_NEGATIVE_INTEGER:
        return visitor.acceptNegativeInteger(content.asInteger);

      case JSON_POSITIVE_INTEGER:
        return visitor.acceptPositiveInteger(content.asInteger);

      case JSON_BOOLEAN:
        return visitor.acceptBoolean(content.asInteger != 0);

      default:  // JSON_UNDEFINED
        return visitor.acceptNull();
    }
  }
};
}  // namespace Internals
}  // namespace ArduinoJson
