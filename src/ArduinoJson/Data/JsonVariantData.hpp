// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

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
};
}  // namespace Internals
}  // namespace ArduinoJson
