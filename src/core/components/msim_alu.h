#ifndef MSIM_ALU_H
#define MSIM_ALU_H

#include "msim_component.h"
#include "core/microcode/inst_word.h"

class msim_alu : public msim_component{

public:
    msim_alu(std::string const & id, std::string const & label);
    void set_operation(uint8_t operation);
    void set_x_value(int value);
    void set_y_value(int value);
    void execute();
    void set_constant_value(int value);
    [[nodiscard]] int get_result() const;

    [[nodiscard]] uint8_t get_flags() const;

private:
    uint8_t m_operation = Z_Z;
    int m_x_value;
    int m_y_value;
    int m_result;
    int m_constant_value;

    /*      Z + - Of */
    uint8_t m_flags;

};

#endif // MSIM_ALU_H
