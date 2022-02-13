#ifndef FSM_HPP
#define FSM_HPP

#include <functional>

template<typename t_state_type, typename t_alphabet_type>
class fsm
{
public:
    using state_type = t_state_type;
    using alphabet_type = t_alphabet_type;
    using transition_function_type = std::function<state_type(state_type, alphabet_type)>;
public:
    fsm(transition_function_type a_delta, state_type a_q0)
        : m_delta(a_delta),
          m_q(a_q0) {}
public:
    fsm() = delete;
public:
    state_type work(alphabet_type const* input_string, std::size_t input_len)
    {
        for(std::size_t i = 0; i < input_len; ++i)
            m_q = m_delta(m_q, input_string[i]);
        return m_q;
    }
private:
    transition_function_type m_delta;
    state_type m_q;
};

#endif
