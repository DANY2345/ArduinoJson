// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include "Data/JsonVariantContent.hpp"
#include "Data/JsonVariantType.hpp"

namespace ArduinoJson {

namespace Internals {
struct JsonVariantData {
  JsonVariantType type;
  JsonVariantContent content;
};
}  // namespace Internals
}  // namespace ArduinoJson
