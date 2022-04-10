#include <catch2/catch_all.hpp>
#include <static_map.hpp>
#include <static_string.hpp>

using namespace noarch::containers;

// enforce code coverage with explicit instances of func templates so that linker does not drop references
template int* StaticMap<int, int, 1>::find_key(const int &key);

/// @brief lookup a valid "Key"; must check for nullptr
TEST_CASE("Static_map - valid lookup", "[static_map]")
{

    // an example enum for map "Keys"
    enum class ExampleKeyType
    {
        ONE,
        TWO,
        THREE,
        FOUR
    };

    // an example type for map "Values"
    struct ExampleValueType
    {
        // constructor/initialiser
        ExampleValueType(int int_value, std::string string_value) : m_int_value(int_value), m_string_value(string_value)
        {
            // no action
        }
        // some integer data member
        int m_int_value;
        // some string data member
        std::string m_string_value;
    };

    // declare our data pair for the map at compile time
    const int value_for_key_one{100};
    const int value_for_key_two{200};
    const int value_for_key_three{300};
    static std::array<std::pair<ExampleKeyType, ExampleValueType>, 3 > data_set
    {{
        { ExampleKeyType::ONE, ExampleValueType(value_for_key_one, "100") },
        { ExampleKeyType::TWO, ExampleValueType(value_for_key_two, "200") },
        { ExampleKeyType::THREE, ExampleValueType(value_for_key_three, "300") }
    }};


    // initialise the StaticMap container with our static allocated data set
    StaticMap<ExampleKeyType, ExampleValueType, data_set.size()> the_map = 
        StaticMap<ExampleKeyType, ExampleValueType, data_set.size()>{{data_set}};   

    // lookup valid key ONE
    ExampleValueType* res_one = the_map.find_key(ExampleKeyType::ONE);
    REQUIRE(res_one != nullptr);
    REQUIRE(res_one->m_int_value == value_for_key_one);
    REQUIRE_FALSE(res_one->m_int_value == value_for_key_two);
    REQUIRE_FALSE(res_one->m_int_value == value_for_key_three);

    // lookup valid key TWO
    ExampleValueType* res_two = the_map.find_key(ExampleKeyType::TWO);
    REQUIRE(res_two != nullptr);
    REQUIRE(res_two->m_int_value == value_for_key_two);
    REQUIRE_FALSE(res_two->m_int_value == value_for_key_one);
    REQUIRE_FALSE(res_two->m_int_value == value_for_key_three);    

    // lookup valid key THREE
    ExampleValueType* res_three = the_map.find_key(ExampleKeyType::THREE);
    REQUIRE(res_three != nullptr);
    REQUIRE(res_three->m_int_value == value_for_key_three);    
    REQUIRE_FALSE(res_three->m_int_value == value_for_key_one);
    REQUIRE_FALSE(res_three->m_int_value == value_for_key_two);    

    // lookup invalid key FOUR
    ExampleValueType* res_four = the_map.find_key(ExampleKeyType::FOUR);
    REQUIRE(res_four == nullptr);    

    
}

