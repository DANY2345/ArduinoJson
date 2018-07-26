// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

using namespace Catch::Matchers;

TEST_CASE("JsonObject::begin()/end()") {
  StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
  JsonObject obj = doc.to<JsonObject>();
  obj["ab"] = 12;
  obj["cd"] = 34;

  SECTION("NonConstIterator") {
    JsonObject::iterator it = obj.begin();
    REQUIRE(obj.end() != it);
    REQUIRE_THAT(static_cast<const char*>(it->key()), Equals("ab"));
    REQUIRE(12 == it->value());
    it->key().set("a.b");
    it->value().set(1.2);
    ++it;
    REQUIRE(obj.end() != it);
    REQUIRE_THAT(static_cast<const char*>(it->key()), Equals("cd"));
    REQUIRE(34 == it->value());
    it->key().set("c.d");
    it->value().set(3.4);
    ++it;
    REQUIRE(obj.end() == it);

    REQUIRE(2 == obj.size());
    REQUIRE(1.2 == obj["a.b"]);
    REQUIRE(3.4 == obj["c.d"]);
  }

  SECTION("ConstIterator") {
    const JsonObject const_object = obj;
    JsonObject::const_iterator it = const_object.begin();

    REQUIRE(const_object.end() != it);
    REQUIRE_THAT(it->key(), Equals("ab"));
    REQUIRE(12 == it->value());
    ++it;
    REQUIRE(const_object.end() != it);
    REQUIRE_THAT(it->key(), Equals("cd"));
    REQUIRE(34 == it->value());
    ++it;
    REQUIRE(const_object.end() == it);
  }
}
