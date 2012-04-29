#include <set>

#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/JSON/DefaultHandler.h"
//#include "Poco/Util/JSONConfiguration.h"
#include "Poco/JSON/Template.h"

#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/Glob.h"

#include "CppUnit/TestCase.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "CppUnit/TestRunner.h"

class JSONTest : public CppUnit::TestCase
{
public:
  JSONTest(const std::string& name): CppUnit::TestCase("JSON")
  {

  }

  void setUp()
  {
  }

  void tearDown()
  {
  }

protected:
  void testNullProperty()
  {
    std::string json = "{ \"test\" : null }";
    Poco::JSON::Parser parser;

    Poco::DynamicAny result;
    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }
    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    assert(object->isNull("test"));
    Poco::DynamicAny test = object->get("test");
    assert(test.isEmpty());
  }

  void testTrueProperty()
  {
    std::string json = "{ \"test\" : true }";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    Poco::DynamicAny test = object->get("test");
    assert(test.type() == typeid(bool));
    bool value = test;
    assert(value);
  }

  void testFalseProperty()
  {
    std::string json = "{ \"test\" : false }";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    Poco::DynamicAny test = object->get("test");
    assert(test.type() == typeid(bool));
    bool value = test;
    assert(!value);
  }

  void testNumberProperty()
  {
    std::string json = "{ \"test\" : 1969 }";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    Poco::DynamicAny test = object->get("test");
    assert(test.isInteger());
    int value = test;
    assert(value == 1969);
  }

  void testStringProperty()
  {
    std::string json = "{ \"test\" : \"value\" }";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    Poco::DynamicAny test = object->get("test");
    assert(test.isString());
    std::string value = test;
    assert(value.compare("value") == 0);
  }

  void testEmptyObject()
  {
    std::string json = "{}";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    assert(object->size() == 0);
  }

  void testDoubleProperty()
  {
    std::string json = "{ \"test\" : 123.45 }";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    Poco::DynamicAny test = object->get("test");
    assert(test.isNumeric());
    double value = test;
    assert(value == 123.45);
  }

  void testDouble2Property()
  {
    std::string json = "{ \"test\" : 12e34 }";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    Poco::DynamicAny test = object->get("test");
    assert(test.isNumeric());
    double value = test;
    assert(value == 12e34);
  }

  void testDouble3Property()
  {
    std::string json = "{ \"test\" : 12e-34 }";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    Poco::DynamicAny test = object->get("test");
    assert(test.isNumeric());
    double value = test;
    assert(value == 12e-34);
  }

  void testObjectProperty()
  {
    std::string json = "{ \"test\" : { \"property\" : \"value\" } }";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    Poco::DynamicAny test = object->get("test");
    assert(test.type() == typeid(Poco::JSON::Object::Ptr));
    object = test.extract<Poco::JSON::Object::Ptr>();

    test = object->get("property");
    assert(test.isString());
    std::string value = test;
    assert(value.compare("value") == 0);
  }

  void testEmptyArray()
  {
    std::string json = "[]";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Array::Ptr));

    Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
    assert(array->size() == 0);
  }

  void testNestedArray()
  {
    std::string json = "[[[[]]]]";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Array::Ptr));

    Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
    assert(array->size() == 1);
  }

  void testNullElement()
  {
    std::string json = "[ null ]";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Array::Ptr));

    Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
    assert(array->isNull(0));
    Poco::DynamicAny test = array->get(0);
    assert(test.isEmpty());
  }

  void testTrueElement()
  {
    std::string json = "[ true ]";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Array::Ptr));

    Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
    Poco::DynamicAny test = array->get(0);
    assert(test.type() == typeid(bool));
    bool value = test;
    assert(value);
  }

  void testFalseElement()
  {
    std::string json = "[ false ]";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Array::Ptr));

    Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
    Poco::DynamicAny test = array->get(0);
    assert(test.type() == typeid(bool));
    bool value = test;
    assert(!value);
  }

  void testNumberElement()
  {
    std::string json = "[ 1969 ]";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Array::Ptr));

    Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
    Poco::DynamicAny test = array->get(0);
    assert(test.isInteger());
    int value = test;
    assert(value == 1969);
  }

  void testStringElement()
  {
    std::string json = "[ \"value\" ]";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Array::Ptr));

    Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
    Poco::DynamicAny test = array->get(0);
    assert(test.isString());
    std::string value = test;
    assert(value.compare("value") == 0);
  }

  void testEmptyObjectElement()
  {
    std::string json = "[{}]";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Array::Ptr));

    Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
    Poco::JSON::Object::Ptr object = array->getObject(0);
    assert(object->size() == 0);
  }

  void testDoubleElement()
  {
    std::string json = "[ 123.45 ]";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Array::Ptr));

    Poco::JSON::Array::Ptr array = result.extract<Poco::JSON::Array::Ptr>();
    Poco::DynamicAny test = array->get(0);
    assert(test.isNumeric());
    double value = test;
    assert(value == 123.45);
  }

  void testOptValue()
  {
    std::string json = "{ }";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    int n = object->optValue("test", 123);
    assert(n == 123);
  }

  void testQuery()
  {
    std::string json = "{ \"name\" : \"Franky\", \"children\" : [ \"Jonas\", \"Ellen\" ] }";
    Poco::JSON::Parser parser;
    Poco::DynamicAny result;

    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(json);
      result = handler.result();
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    assert(result.type() == typeid(Poco::JSON::Object::Ptr));

    Poco::JSON::Query query(result);

    std::string firstChild = query.findValue("children[0]", "");
    assert(firstChild.compare("Jonas") == 0);
  }

  void testValidJanssonFiles()
  {
    Poco::Path pathPattern("/home/bronx/Development/mqweb/JSON/testsuite/testfiles/valid/*");
    std::set<std::string> paths;
    Poco::Glob::glob(pathPattern, paths);

    for(std::set<std::string>::iterator it = paths.begin(); it != paths.end(); ++it)
    {
      Poco::Path filePath(*it, "input");

      if ( filePath.isFile() )
      {
        Poco::File inputFile(filePath);
        if ( inputFile.exists() )
        {
          Poco::FileInputStream fis(filePath.toString());
          std::cout << filePath.toString() << std::endl;

          Poco::JSON::Parser parser;
          Poco::DynamicAny result;

          try
          {
            Poco::JSON::DefaultHandler handler;
            parser.setHandler(&handler);
            parser.parse(fis);
            result = handler.result();
            std::cout << "Ok!" << std::endl;
          }
          catch(Poco::JSON::JSONException jsone)
          {
            // We shouldn't get here.
            assert(false);
          }
        }
      }
    }
  }

  void testInvalidJanssonFiles()
  {
    Poco::Path pathPattern("/home/bronx/Development/mqweb/JSON/testsuite/testfiles/invalid/*");
    std::set<std::string> paths;
    Poco::Glob::glob(pathPattern, paths);

    for(std::set<std::string>::iterator it = paths.begin(); it != paths.end(); ++it)
    {
      Poco::Path filePath(*it, "input");

      if ( filePath.isFile() )
      {
        Poco::File inputFile(filePath);
        if ( inputFile.exists() )
        {
          Poco::FileInputStream fis(filePath.toString());
          std::cout << filePath.toString() << std::endl;

          Poco::JSON::Parser parser;
          Poco::DynamicAny result;

          try
          {
            Poco::JSON::DefaultHandler handler;
            parser.setHandler(&handler);
            parser.parse(fis);
            result = handler.result();
          }
          catch(Poco::JSON::JSONException jsone)
          {
            std::cout << "Exception!!! " << jsone.message() << std::endl;
          }
        }
      }
    }
  }
/*
  void testConfiguration()
  {
    Poco::JSON::Configuration config;

    std::string json = "{ \"config\" : "
                        " { \"prop1\" : \"value1\", "
                           " \"prop2\" : 10, "
                           " \"prop3\" : [ \"element1\", \"element2\" ], "
                          " \"prop4\" : { \"prop5\" : false, "
                                        " \"prop6\" : null } "
                                        "}"
                       "}";

    std::istringstream iss(json);
    try
    {
      config.load(iss);
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message() << std::endl;
      assert(false);
    }

    std::string property1 = config.getString("config.prop1");
    assert(property1.compare("value1") == 0);

    int property2 = config.getInt("config.prop2");
    assert(property2 == 10);

    int nonExistingProperty = config.getInt("config.prop7", 5);
    assert(nonExistingProperty == 5);

    std::string arrProperty = config.getString("config.prop3[1]");
    assert(arrProperty.compare("element2") == 0);

    bool property35 = config.getBool("config.prop4.prop5");
    assert(! property35);

    try
    {
      config.getString("propertyUnknown");
      assert(true);
    }
    catch(Poco::NotFoundException nfe)
    {
    }

  }
*/
  void testTemplate()
  {
    Poco::JSON::Template tpl;
    tpl.parse("Hello world! From <?= person.name ?>\n<?if person.toOld ?>You're to old<?endif?>\n");

    Poco::JSON::Object::Ptr data = new Poco::JSON::Object();
    Poco::JSON::Object::Ptr person = new Poco::JSON::Object();
    data->set("person", person);
    person->set("name", "Franky");
    person->set("toOld", true);
    tpl.render(data, std::cout);
  }

  void testItunes()
  {
    Poco::FileInputStream fis("/home/bronx/Development/search.json");
    Poco::JSON::Parser parser;
    try
    {
      Poco::JSON::DefaultHandler handler;
      parser.setHandler(&handler);
      parser.parse(fis);
    }
    catch(Poco::JSON::JSONException jsone)
    {
      std::cout << jsone.message();
    }
  }

public:
  static CppUnit::Test* suite()
  {
    CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JSONTest");

    CppUnit_addTest(pSuite, JSONTest, testNullProperty);
    CppUnit_addTest(pSuite, JSONTest, testTrueProperty);
    CppUnit_addTest(pSuite, JSONTest, testFalseProperty);
    CppUnit_addTest(pSuite, JSONTest, testNumberProperty);
    CppUnit_addTest(pSuite, JSONTest, testStringProperty);
    CppUnit_addTest(pSuite, JSONTest, testEmptyObject);
    CppUnit_addTest(pSuite, JSONTest, testDoubleProperty);
    CppUnit_addTest(pSuite, JSONTest, testDouble2Property);
    CppUnit_addTest(pSuite, JSONTest, testDouble3Property);
    CppUnit_addTest(pSuite, JSONTest, testObjectProperty);
    CppUnit_addTest(pSuite, JSONTest, testEmptyArray);
    CppUnit_addTest(pSuite, JSONTest, testNestedArray);
    CppUnit_addTest(pSuite, JSONTest, testNullElement);
    CppUnit_addTest(pSuite, JSONTest, testTrueElement);
    CppUnit_addTest(pSuite, JSONTest, testFalseElement);
    CppUnit_addTest(pSuite, JSONTest, testNumberElement);
    CppUnit_addTest(pSuite, JSONTest, testStringElement);
    CppUnit_addTest(pSuite, JSONTest, testEmptyObjectElement);
    CppUnit_addTest(pSuite, JSONTest, testDoubleElement);
    CppUnit_addTest(pSuite, JSONTest, testOptValue);
    CppUnit_addTest(pSuite, JSONTest, testQuery);
    //CppUnit_addTest(pSuite, JSONTest, testValidJanssonFiles);
    //CppUnit_addTest(pSuite, JSONTest, testInvalidJanssonFiles);

    //CppUnit_addTest(pSuite, JSONTest, testConfiguration);
    CppUnit_addTest(pSuite, JSONTest, testTemplate);

    //CppUnit_addTest(pSuite, JSONTest, testItunes);

    return pSuite;
  }
};

CppUnitMain(JSONTest)
