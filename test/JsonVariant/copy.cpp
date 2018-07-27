// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

TEST_CASE("JsonVariant copy") {
  DynamicJsonDocument doc1, doc2;
  JsonVariant variant1 = doc1.to<JsonVariant>();
  JsonVariant variant2 = doc2.to<JsonVariant>();

  SECTION("IntegersAreCopiedByValue") {
    variant1.set(123);
    variant2 = variant1;
    variant1.set(456);

    REQUIRE(123 == variant2.as<int>());
  }

  SECTION("DoublesAreCopiedByValue") {
    variant1.set(123.45);
    variant2 = variant1;
    variant1.set(456.78);

    REQUIRE(123.45 == variant2.as<double>());
  }

  SECTION("BooleansAreCopiedByValue") {
    variant1.set(true);
    variant2 = variant1;
    variant1.set(false);

    REQUIRE(variant2.as<bool>());
  }

  SECTION("StringsAreCopiedByValue") {
    variant1.set("hello");
    variant2 = variant1;
    variant1.set("world");

    REQUIRE(std::string("hello") == variant2.as<const char*>());
  }

  SECTION("ObjectsAreCopiedByReference") {
    DynamicJsonDocument doc;
    JsonObject object = doc.to<JsonObject>();

    variant1.set(object);

    object["hello"] = "world";

    REQUIRE(1 == variant1.as<JsonObject>().size());
  }

  SECTION("ArraysAreCopiedByReference") {
    DynamicJsonDocument doc;
    JsonArray array = doc.to<JsonArray>();

    variant1.set(array);

    array.add("world");

    REQUIRE(1 == variant1.as<JsonArray>().size());
  }
}
