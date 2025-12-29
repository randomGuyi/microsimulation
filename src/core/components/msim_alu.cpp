#include "msim_alu.h"

msim_alu::msim_alu(std::string const & id, std::string const & label)
    : msim_component{id, label}
, m_operation{Z_Z}
, m_x_value{0}
, m_y_value{0}
, m_result{0}
, m_constant_value{0}
{}

void msim_alu::set_operation(uint8_t operation) {
    m_operation = operation;
}

void msim_alu::set_x_value(int value) {
    m_x_value = value;
}

void msim_alu::set_y_value(int value) {
    m_y_value = value;
}

void msim_alu::execute() {
    switch (m_operation) {
        case Z_Z: {
            return;
        }
        case Z_CONST: {
            m_result = m_constant_value;
            break;
        }
        case Z_X: {
            m_result = m_x_value;
            break;
        }
        case Z_Y: {
            m_result = m_y_value;
            break;
        }
        case Z_X_PLS_Y: {
            m_result = m_x_value + m_y_value;
            break;
        }
        case Z_INC_X: {
            m_result = m_x_value + 1;
            break;
        }
        case Z_INC_Y: {
            m_result = m_y_value + 1;
            break;
        }
        case Z_X_TIMES_Y: {
            m_result = m_x_value * m_y_value;
            break;
        }
        case Z_X_MINUS_Y: {
            m_result = m_x_value - m_y_value;
            break;
        }
        case Z_DEC_X: {
            m_result = m_x_value - 1;
            break;
        }
        case Z_DEC_Y: {
            m_result = m_y_value - 1;
            break;
        }
        case Z_X_DIV_Y: {
            m_result = m_y_value != 0 ? m_x_value / m_y_value : 0;
            break;
        }
        case Z_X_SHL_Y: {
            m_result = m_x_value << m_y_value;
            break;
        }
        case Z_MINUS_X: {
            m_result = -m_x_value;
            break;
        }
        default: /* noting to do */ ;
            break;
    }
    notify({get_mux_selection(), m_result});
}


void msim_alu::set_constant_value(int value) {
    m_constant_value = value;
}

int msim_alu::get_result() const {
    return m_result;
}

uint8_t msim_alu::get_flags() const {
    /*      Z + - Of */
    if (m_result == 0 ) return 0b00001000;
    if (m_result > 0 ) return 0b00000100;
    return 0b00000010; // < 0
    // todo: implement logic for 16 bit max
}


uint8_t msim_alu::get_operation() const {
    return m_operation;
}

mux_selection msim_alu::get_mux_selection() const {
    switch (m_operation) {
        case Z_X:
        case Z_Y:
        case Z_X_PLS_Y:
        case Z_INC_X:
        case Z_INC_Y:
        case Z_X_TIMES_Y:
        case Z_X_MINUS_Y:
        case Z_DEC_X:
        case Z_DEC_Y:
        case Z_X_DIV_Y:
        case Z_MINUS_X:
            return mux_selection::AU_RESULT;
        case Z_CONST:
            return mux_selection::CONSTANT;
        default:
            return mux_selection::LU_RESULT;
    }

}
