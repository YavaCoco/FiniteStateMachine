//
// --> A --|--'c'--> B --'a'--> C --'t'--> (D) --|--*--> (*D)
//         |--'d'--> (*C)                        |--'t'-> *A
//

#include <regex>
#include <vector>
#include <chrono>
#include <cstring>
#include <iostream>
#include <algorithm>

#include "fsm.hpp"


// -----FSM
enum class states
{
    A, B, C, D, DEAD
};

using my_fsm_type = fsm<states, char>;

my_fsm_type::state_type transition_func(my_fsm_type::state_type q, my_fsm_type::alphabet_type s)
{
    switch(q)
    {
        case my_fsm_type::state_type::A:
            switch(s)
            {
                case 'c': return my_fsm_type::state_type::B;
                case 'd': return my_fsm_type::state_type::C;
                default : return my_fsm_type::state_type::DEAD;
            }
        case my_fsm_type::state_type::B:
            switch(s)
            {
                case 'a': return my_fsm_type::state_type::C;
                default : return my_fsm_type::state_type::DEAD;
            }
        case my_fsm_type::state_type::C:
            switch(s)
            {
                case 't': return my_fsm_type::state_type::D;
                default : return my_fsm_type::state_type::DEAD;
            }
        case my_fsm_type::state_type::D:
            switch(s)
            {
                case 't': return my_fsm_type::state_type::A;
                default : return my_fsm_type::state_type::D;
            }
        case my_fsm_type::state_type::DEAD:
            return my_fsm_type::state_type::DEAD;
    }
}

bool test_finite_state_machine(char const* input)
{
    my_fsm_type machine(transition_func, my_fsm_type::state_type::A);

    my_fsm_type::state_type end_state = machine.work(input, std::strlen(input));

    switch(end_state)
    {
        case my_fsm_type::state_type::D: return true;
        default                        : return false;
    }
}
// -----FSM END

// -----REGEX
std::regex expr("^(d|ca)t[^t]*(t(d|ca)t[^t]*)*$");
bool test_std_regex(char const* input)
{
    return std::regex_match(input, expr);
}
// -----REGEX END

// -- MEASURE
using duration_type = std::chrono::duration<float>;
using test_func = std::function<bool(char const*)>;
using measurement = std::pair<std::string, duration_type>;

constexpr std::size_t LOOP_COUNT = 65356;

duration_type measure(test_func func, char const* input)
{
    auto t0 = std::chrono::high_resolution_clock::now();
    for(std::size_t i = 0; i < LOOP_COUNT; ++i)
        func(input);
    auto t1 = std::chrono::high_resolution_clock::now();
    return t1 - t0;
}

bool compare_measurements(measurement& a, measurement& b)
{
    return a.second.count() < b.second.count();
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_string>" << std::endl;
        std::exit(1);
    }
    std::vector<std::pair<std::string, duration_type>> results;
    
    results.push_back({ "FSM"  , measure(test_finite_state_machine, argv[1]) });
    results.push_back({ "REGEX", measure(test_std_regex           , argv[1]) });

    std::sort(results.begin(), results.end(), compare_measurements);

    for(measurement const& measurement : results)
        std::cout << measurement.first << ": " << measurement.second.count() * 1000 / LOOP_COUNT << "ms" << std::endl;

}
