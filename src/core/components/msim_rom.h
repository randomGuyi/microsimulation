#ifndef MSIM_ROM_H
#define MSIM_ROM_H

#include "msim_component.h"

#include <vector>
#include <memory>

#include "../microcode/inst_word.h"


class msim_rom : public msim_component{
public:
    msim_rom(std::string const & id, std::string const & label);
    msim_rom & operator=(const msim_rom & other) = delete;
    msim_rom (const msim_rom & other) = delete;

    void add_inst(inst_word && i_word, int line);
    void next();
    bool has_next() const;

    void back();
    bool has_prev() const;

    bool is_valid_line(int line) const;
    void reset_to_line(int line);

    inst_word const * get_current_instruction() const ;
    std::vector<std::unique_ptr<inst_word>> const & get_all_instructions() const;

    int get_current_line() const;

    bool is_at_first_instruction() const;
    bool is_at_last_instruction() const;

   // virtual ~msim_rom() override;


private:

    /* TODO: change to static (in memory), so performace can be
     * improved, if its slow.... */

    std::vector<std::unique_ptr<inst_word> > m_instructions;
    int m_index;
    int m_size;


};

#endif // MSIM_ROM_H
