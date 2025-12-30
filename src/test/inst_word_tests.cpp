//
// Created by Arthur on 26.12.25.
//






#include <iostream>
#include <sstream>
#include <string>

#include "../core/components/msim_component.h"
#include "../core/microcode/inst_word.h"
#include "../core/parser/msim_parser.h"

static int g_tests = 0;
static int g_fails = 0;
using namespace core;
template<typename T, typename U>
bool expect_eq(const char* name, const T& expected, const U& actual) {
    ++g_tests;
    if (expected == actual) {
        std::cout << "[PASS] " << name << std::endl;
        return true;
    } else {
        ++g_fails;
        std::cout << "[FAIL] " << name << " expected=" << typeid(expected).name() << " actual=" << typeid(actual).name() << std::endl;
        return false;
    }
}

bool test_defaults() {
    core::inst_word iw;
    bool ok = true;
    ok &= expect_eq("default raw word == 0", uint32_t(0), iw.get_raw_word());
    ok &= expect_eq("default ram mode == WAIT", ram_mode::WAIT, iw.get_ram_mode());
    ok &= expect_eq("default ar mode == CHAR_PLS_PLS", ar_mode::CHAR_PLS_PLS, iw.get_ar_mode());
    return ok;
}

bool test_selection_and_operation_and_flags() {
    inst_word iw;
    bool ok = true;

    iw.set_x_selection(0b101);
    iw.set_y_selection(0b011);
    iw.set_z_selection(0b001);

    ok &= expect_eq("x selection", uint8_t(0b101), iw.get_x_selection());
    ok &= expect_eq("y selection", uint8_t(0b011), iw.get_y_selection());
    ok &= expect_eq("z selection", uint8_t(0b001), iw.get_z_selection());

    iw.set_operation(Z_X);
    std::cout << iw << std::endl;
    ok &= expect_eq("operation Z_X", uint8_t(Z_X), iw.get_operation());

    // flags true
    iw.set_z_mar(true);
    iw.set_z_mdr(true);
    iw.set_mdr_y(true);
    iw.set_mdr_cop(true);
    ok &= expect_eq("z_mar true", true, iw.get_z_mar());
    ok &= expect_eq("z_mdr true", true, iw.get_z_mdr());
    ok &= expect_eq("mdr_y true", true, iw.get_mdr_y());
    ok &= expect_eq("mdr_cop true", true, iw.get_mdr_cop());

    // flags false
    iw.set_z_mar(false);
    iw.set_z_mdr(false);
    iw.set_mdr_y(false);
    iw.set_mdr_cop(false);
    ok &= expect_eq("z_mar false", false, iw.get_z_mar());
    ok &= expect_eq("z_mdr false", false, iw.get_z_mdr());
    ok &= expect_eq("mdr_y false", false, iw.get_mdr_y());
    ok &= expect_eq("mdr_cop false", false, iw.get_mdr_cop());
    return ok;
}

bool test_ram_and_ar_modes() {
    inst_word iw;
    bool ok = true;

    iw.set_ram_mode(ram_mode::READ);
    ok &= expect_eq("ram mode READ", ram_mode::READ, iw.get_ram_mode());

    iw.set_ram_mode(ram_mode::WRITE);
    ok &= expect_eq("ram mode WRITE", ram_mode::WRITE, iw.get_ram_mode());

    // AR modes
    iw.set_ar_mode(ar_mode::CHAR_PLS_PLS);
    ok &= expect_eq("ar mode CHAR_PLS_PLS", ar_mode::CHAR_PLS_PLS, iw.get_ar_mode());

    iw.set_ar_mode(ar_mode::_4COP);
    ok &= expect_eq("ar mode _4COP", ar_mode::_4COP, iw.get_ar_mode());

    iw.set_ar_mode(ar_mode::_4COP_CND);
    ok &= expect_eq("ar mode _4COP_CND", ar_mode::_4COP_CND, iw.get_ar_mode());

    iw.set_ar_mode(ar_mode::_4CN);
    ok &= expect_eq("ar mode _4CN", ar_mode::_4CN, iw.get_ar_mode());

    // default ar opt write/read
    iw.set_default_ar_opt();
    ok &= expect_eq("default ar opt", ar_mode::CHAR_PLS_PLS, iw.get_ar_mode());

    return ok;
}

bool test_mask_cn_and_constants() {
    inst_word iw;
    bool ok = true;

    iw.set_mask(0b1010);
    ok &= expect_eq("mask 0b1010", uint8_t(0b1010), iw.get_mask());

    iw.set_cn(0b1010101);
    ok &= expect_eq("cn 0b1010101", int(0b1010101), iw.get_cn());

    // constant number: second set should produce an error message
    iw.set_constant_nbr(7);
    iw.set_constant_nbr(8); // should add an error message
    std::string em = iw.err_msg();
    bool contains = em.find("Constant number already set") != std::string::npos;
    ok &= expect_eq("constant second-set produces error", true, contains);

    return ok;
}

bool test_operator_stream() {
    inst_word iw;
    bool ok = true;
    iw.set_x_selection(1);
    std::stringstream ss;
    ss << iw;
    std::string out = ss.str();
    // basic checks: should contain '|' and a newline
    ok &= expect_eq("operator<< contains pipe", true, out.find('|') != std::string::npos);
    ok &= expect_eq("operator<< contains newline", true, out.find('\n') != std::string::npos);
    return ok;
}

int run_tests() {
    std::cout << "Running inst_word tests..." << std::endl;
    bool all = true;
    all &= test_defaults();
    all &= test_selection_and_operation_and_flags();
    all &= test_ram_and_ar_modes();
    all &= test_mask_cn_and_constants();
    all &= test_operator_stream();

    std::cout << "Tests run: " << g_tests << ", Failures: " << g_fails << std::endl;
    if (g_fails == 0 && all) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}
