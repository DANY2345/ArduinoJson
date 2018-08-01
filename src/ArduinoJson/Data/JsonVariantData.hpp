// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "../Numbers/parseFloat.hpp"
#include "../Numbers/parseInteger.hpp"
#include "../Polyfills/type_traits.hpp"
#include "../SerializedValue.hpp"
#include "JsonVariantContent.hpp"
#include "JsonVariantType.hpp"

namespace ArduinoJson {
namespace Internals {

struct JsonVariantData {
  JsonVariantType type;
  JsonVariantContent content;

  void set(bool value) {
    using namespace Internals;
    type = JSON_BOOLEAN;
    content.asInteger = static_cast<JsonUInt>(value);
  }

  // Create a JsonVariant containing a floating point value.
  // set(double value);
  // set(float value);
  template <typename T>
  void set(T value,
           typename enable_if<is_floating_point<T>::value>::type * = 0) {
    using namespace Internals;
    type = JSON_FLOAT;
    content.asFloat = static_cast<JsonFloat>(value);
  }

  // Create a JsonVariant containing an integer value.
  // set(char)
  // set(signed short)
  // set(signed int)
  // set(signed long)
  // set(signed char)
  template <typename T>
  void set(T value, typename enable_if<is_integral<T>::value &&
                                       is_signed<T>::value>::type * = 0) {
    using namespace Internals;
    if (value >= 0) {
      type = JSON_POSITIVE_INTEGER;
      content.asInteger = static_cast<JsonUInt>(value);
    } else {
      type = JSON_NEGATIVE_INTEGER;
      content.asInteger = ~static_cast<JsonUInt>(value) + 1;
    }
  }
  // set(unsigned short)
  // set(unsigned int)
  // set(unsigned long)
  template <typename T>
  void set(T value, typename enable_if<is_integral<T>::value &&
                                       is_unsigned<T>::value>::type * = 0) {
    using namespace Internals;
    type = JSON_POSITIVE_INTEGER;
    content.asInteger = static_cast<JsonUInt>(value);
  }

  // Create a JsonVariant containing a string.
  // set(const char*);
  // set(const signed char*);
  // set(const unsigned char*);
  template <typename TChar>
  void set(const TChar *value,
           typename enable_if<sizeof(TChar) == 1>::type * = 0) {
    type = JSON_STRING;
    content.asString = reinterpret_cast<const char *>(value);
  }

  // Create a JsonVariant containing an unparsed string
  void set(SerializedValue<const char *> value) {
    type = JSON_UNPARSED;
    content.asRaw.data = value.data();
    content.asRaw.size = value.size();
  }

  void set(const JsonVariantData *value) {
    if (value) {
      type = value->type;
      content = value->content;
    } else {
      type = JSON_UNDEFINED;
    }
  }

  void set(JsonArrayData *array) {
    if (array) {
      type = JSON_ARRAY;
      content.asArray = array;
    } else {
      type = JSON_UNDEFINED;
    }
  }

  void set(JsonObjectData *&object) {
    if (object) {
      type = JSON_OBJECT;
      content.asObject = object;
    } else {
      type = JSON_UNDEFINED;
    }
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
