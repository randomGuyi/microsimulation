#ifndef MSIM_ROM_H
#define MSIM_ROM_H

#include "msim_component.h"

#include <vector>
#include <memory>

#include "msim_observable_component.h"
#include "../microcode/inst_word.h"

namespace core::components {
    class msim_rom : public msim_component
                                    /* instructions, current index */
        ,  public msim_observable_component<int>{
    public:
        msim_rom(std::string const & id, std::string const & label);
        msim_rom & operator=(const msim_rom & other) = delete;
        msim_rom (const msim_rom & other) = delete;

        void add_inst(inst_word && i_word, int line);
        void next();
        void back();
        void reset_to_line(int line);

        [[nodiscard]] bool has_next() const;
        [[nodiscard]] bool has_prev() const;
        [[nodiscard]] bool is_valid_line(int line) const;
        [[nodiscard]] inst_word const *get_current_instruction() const;
        [[nodiscard]] std::vector<std::unique_ptr<inst_word> > const &get_all_instructions() const;
        [[nodiscard]] int get_current_line() const;
        [[nodiscard]] bool is_at_first_instruction() const;
        [[nodiscard]] bool is_at_last_instruction() const;

        // virtual ~msim_rom() override;


    private:

        /* TODO: change to static (in memory), so performace can be
         * improved, if its slow.... */

        std::vector<std::unique_ptr<inst_word> > m_instructions;
        int m_index;
        int m_size;


    };
}
#endif // MSIM_ROM_H
