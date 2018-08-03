// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>
#include <stdint.h>  // for uint8_t

#include "Data/JsonVariantData.hpp"
#include "Data/JsonVariantDefault.hpp"
#include "JsonVariant.hpp"
#include "JsonVariantBase.hpp"
#include "Memory/JsonBuffer.hpp"
#include "Polyfills/type_traits.hpp"
#include "Serialization/DynamicStringWriter.hpp"
#include "SerializedValue.hpp"

namespace ArduinoJson {

// Forward declarations.
class JsonArray;
class JsonObject;

// A variant that can be a any value serializable to a JSON value.
//
// It can be set to:
// - a boolean
// - a char, short, int or a long (signed or unsigned)
// - a string (const char*)
// - a reference to a JsonArray or JsonObject
class JsonVariant : public Internals::JsonVariantBase<JsonVariant> {
 public:
  // Intenal use only
  explicit JsonVariant(Internals::JsonBuffer *buffer,
                       Internals::JsonVariantData *data)
      : _buffer(buffer), _data(data) {}

  // Creates an uninitialized JsonVariant
  JsonVariant() : _buffer(0), _data(0) {}

  // set(bool value)
  void set(bool value) {
    if (!_data) return;
    _data->setBoolean(value);
  }

  // set(double value);
  // set(float value);
  template <typename T>
  void set(T value, typename Internals::enable_if<
                        Internals::is_floating_point<T>::value>::type * = 0) {
    if (!_data) return;
    _data->setFloat(static_cast<Internals::JsonFloat>(value));
  }

  // set(char)
  // set(signed short)
  // set(signed int)
  // set(signed long)
  // set(signed char)
  template <typename T>
  void set(T value,
           typename Internals::enable_if<Internals::is_integral<T>::value &&
                                         Internals::is_signed<T>::value>::type
               * = 0) {
    if (!_data) return;
    if (value >= 0)
      _data->setPostiveInteger(static_cast<Internals::JsonUInt>(value));
    else
      _data->setNegativeInteger(~static_cast<Internals::JsonUInt>(value) + 1);
  }

  // set(unsigned short)
  // set(unsigned int)
  // set(unsigned long)
  template <typename T>
  void set(T value,
           typename Internals::enable_if<Internals::is_integral<T>::value &&
                                         Internals::is_unsigned<T>::value>::type
               * = 0) {
    if (!_data) return;
    _data->setPostiveInteger(static_cast<Internals::JsonUInt>(value));
  }

  // set(SerializedValue<const char *>)
  void set(Internals::SerializedValue<const char *> value) {
    if (!_data) return;
    _data->setRaw(value.data(), value.size());
  }

  // set(SerializedValue<std::string>)
  // set(SerializedValue<String>)
  // set(SerializedValue<const __FlashStringHelper*>)
  template <typename T>
  void set(Internals::SerializedValue<T> value,
           typename Internals::enable_if<
               !Internals::is_same<const char *, T>::value>::type * = 0) {
    if (!_data) return;
    const char *dup =
        Internals::makeString(value.data(), value.size()).save(_buffer);
    if (dup)
      _data->setRaw(dup, value.size());
    else
      _data->setUndefined();
  }

  // set(const std::string&)
  // set(const String&)
  template <typename T>
  void set(const T &value,
           typename Internals::enable_if<Internals::IsString<T>::value>::type
               * = 0) {
    if (!_data) return;
    const char *dup = Internals::makeString(value).save(_buffer);
    if (dup)
      _data->setString(dup);
    else
      _data->setUndefined();
  }

  // set(const char*);
  // set(const signed char*);
  // set(const unsigned char*);
  template <typename TChar>
  void set(const TChar *value,
           typename Internals::enable_if<sizeof(TChar) == 1>::type * = 0) {
    if (!_data) return;
    _data->setString(reinterpret_cast<const char *>(value));
  }

  void set(JsonVariant value) {
    if (!_data) return;
    if (value._data)
      *_data = *value._data;
    else
      _data->setUndefined();
  }

  void set(const JsonArray &array);
  void set(const Internals::JsonArraySubscript &);
  void set(const JsonObject &object);
  template <typename TString>
  void set(const Internals::JsonObjectSubscript<TString> &);

  // Get the variant as the specified type.
  //
  // char as<char>() const;
  // signed char as<signed char>() const;
  // signed short as<signed short>() const;
  // signed int as<signed int>() const;
  // signed long as<signed long>() const;
  // unsigned char as<unsigned char>() const;
  // unsigned short as<unsigned short>() const;
  // unsigned int as<unsigned int>() const;
  // unsigned long as<unsigned long>() const;
  template <typename T>
  const typename Internals::enable_if<Internals::is_integral<T>::value, T>::type
  as() const {
    return _data ? _data->asInteger<T>() : 0;
  }
  // bool as<bool>() const
  template <typename T>
  const typename Internals::enable_if<Internals::is_same<T, bool>::value,
                                      T>::type
  as() const {
    return _data && _data->asInteger<int>() != 0;
  }
  //
  // double as<double>() const;
  // float as<float>() const;
  template <typename T>
  const typename Internals::enable_if<Internals::is_floating_point<T>::value,
                                      T>::type
  as() const {
    return _data ? _data->asFloat<T>() : 0;
  }
  //
  // const char* as<const char*>() const;
  // const char* as<char*>() const;
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, const char *>::value ||
                                    Internals::is_same<T, char *>::value,
                                const char *>::type
  as() const {
    return _data ? _data->asString() : 0;
  }
  //
  // std::string as<std::string>() const;
  // String as<String>() const;
  template <typename T>
  typename Internals::enable_if<Internals::IsWriteableString<T>::value, T>::type
  as() const {
    const char *cstr = _data ? _data->asString() : 0;
    if (cstr) return T(cstr);
    T s;
    serializeJson(*this, s);
    return s;
  }
  //
  // JsonArray as<JsonArray>() const;
  // const JsonArray as<const JsonArray>() const;
  template <typename T>
  typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonArray>::value,
      JsonArray>::type
  as() const;
  //
  // JsonObject as<JsonObject>() const;
  // const JsonObject as<const JsonObject>() const;
  template <typename T>
  typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonObject>::value,
      T>::type
  as() const;
  //
  // JsonVariant as<JsonVariant> const;
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, JsonVariant>::value,
                                T>::type
  as() const {
    return *this;
  }

  // Tells weither the variant has the specified type.
  // Returns true if the variant has type type T, false otherwise.
  //
  // bool is<char>() const;
  // bool is<signed char>() const;
  // bool is<signed short>() const;
  // bool is<signed int>() const;
  // bool is<signed long>() const;
  // bool is<unsigned char>() const;
  // bool is<unsigned short>() const;
  // bool is<unsigned int>() const;
  // bool is<unsigned long>() const;
  template <typename T>
  typename Internals::enable_if<Internals::is_integral<T>::value, bool>::type
  is() const {
    return _data && _data->isInteger();
  }
  //
  // bool is<double>() const;
  // bool is<float>() const;
  template <typename T>
  typename Internals::enable_if<Internals::is_floating_point<T>::value,
                                bool>::type
  is() const {
    return _data && _data->isFloat();
  }
  //
  // bool is<bool>() const
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, bool>::value, bool>::type
  is() const {
    return _data && _data->isBoolean();
  }
  //
  // bool is<const char*>() const;
  // bool is<char*>() const;
  template <typename T>
  typename Internals::enable_if<Internals::is_same<T, const char *>::value ||
                                    Internals::is_same<T, char *>::value,
                                bool>::type
  is() const {
    return _data && _data->isString();
  }
  //
  // bool is<JsonArray> const;
  // bool is<const JsonArray> const;
  template <typename T>
  typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonArray>::value,
      bool>::type
  is() const {
    return _data && _data->isArray();
  }
  //
  // bool is<JsonObject> const;
  // bool is<const JsonObject> const;
  template <typename T>
  typename Internals::enable_if<
      Internals::is_same<typename Internals::remove_const<T>::type,
                         JsonObject>::value,
      bool>::type
  is() const {
    return _data && _data->isObject();
  }

  // Returns true if the variant has a value
  bool isNull() const {
    return _data == 0 || _data->type == Internals::JSON_UNDEFINED;
  }

  template <typename Visitor>
  void visit(Visitor &visitor) const {
    if (_data)
      _data->visit(visitor);
    else
      visitor.acceptNull();
  }

 private:
  Internals::JsonBuffer *_buffer;
  Internals::JsonVariantData *_data;
};  // namespace ArduinoJson
}  // namespace ArduinoJson
