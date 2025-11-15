//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_MICROCODELEXER_H
#define MICROSIMULATION_MICROCODELEXER_H

#include "core/parser/msim_scanner.h"

#include <Qsci/qscilexercustom.h>
#include <map>

#define LANGUAGE_HINT "Microcode"

class MicrocodeLexer : public QsciLexerCustom{
    Q_OBJECT

public:
    MicrocodeLexer( QObject * parent = nullptr);
    ~MicrocodeLexer() override;

    enum Styles{
        Default 	,
        Number,
        Register,
        Bus,
        Operation,
        Car,
        Comment,
        Error
    };

protected:
    const char * language() const override;
    void styleText(int start, int end) override;
    QString description(int style) const override;


private:
    std::map<token_type, Styles> token_to_syle = {

    /* default */
    {token_type::COND_NE0_SY, Styles::Default }, 		// "cond<>0"
    {token_type::WRITE_SY, Styles::Default },   		// "write"
    {token_type::READ_SY, Styles::Default },  			// "read"
    {token_type::WAIT_SY, Styles::Default },     		// "wait"
    {token_type::RAM_SY, Styles::Default }, 			// "RAM:"
    {token_type::H_SY, Styles::Default }, 				// "H:"
    {token_type::B_SY, Styles::Default }, 				// "B:"
    {token_type::R_SY, Styles::Default }, 				// "R:"
    {token_type::SEMICOLON_SY, Styles::Default },  		// "\n"
    {token_type::COMMA_SY, Styles::Default }, 			// ","

    /* number */
    {token_type::HASH_SY,Styles::Number },  			// "#"
    {token_type::SLASH_SY,Styles::Number }, 			// "/"
    {token_type::NUMBER_SY,Styles::Number },

    /* register */
    {token_type::MDR_TO_COP_SY, Styles::Register }, 		// "MDR->COP"
    {token_type::MDR_TO_Y_SY, Styles::Register }, 		// "MDR->Y"
    {token_type::R0_TO_X_SY, Styles::Register }, 		// "R0->X"
    {token_type::R1_TO_X_SY, Styles::Register }, 		// "R1->X"
    {token_type::R2_TO_X_SY, Styles::Register }, 		// "R2->X"
    {token_type::R3_TO_X_SY, Styles::Register }, 		// "R3->X"
    {token_type::R0_TO_Y_SY, Styles::Register }, 		// "R0->Y"
    {token_type::R1_TO_Y_SY, Styles::Register }, 		// "R1->Y"
    {token_type::R2_TO_Y_SY, Styles::Register }, 		// "R2->Y"
    {token_type::R3_TO_Y_SY, Styles::Register }, 		// "R3->Y"

    /* bus */
    {token_type::Z_MAR_SY, Styles::Bus }, 			// "Z->MAR"
    {token_type::Z_MDR_SY, Styles::Bus }, 		    // "Z->MDR"
    {token_type::Z_R0_SY, Styles::Bus }, 			// "Z->R0"
    {token_type::Z_R1_SY, Styles::Bus }, 			// "Z->R1"
    {token_type::Z_R2_SY, Styles::Bus },  			// "Z->R2"
    {token_type::Z_R3_SY, Styles::Bus }, 			// "Z->R3"
    {token_type::Z_SY, Styles::Bus }, 				// "Z"
    {token_type::X_SY, Styles::Bus }, 				// "X"
    {token_type::Y_SY, Styles::Bus }, 				// "Y"

    /* Operation */
    {token_type::INC_SY, Styles::Operation }, 			// "INC"
    {token_type::DEC_SY, Styles::Operation }, 			// "DEC"
    {token_type::SHL_Y_SY, Styles::Operation }, 			// "<<Y"
    {token_type::AR_SY, Styles::Operation }, 				// "AR:"
    {token_type::_4_CN_SY, Styles::Operation }, 			// "4CN"
    {token_type::CN_ASSIGN_SY, Styles::Operation },   	// "CN="
    {token_type::Z_ASSIGN_SY, Styles::Operation }, 		// "Z:="
    {token_type::MIN_X_SY, Styles::Operation }, 			// "-X"
    {token_type::PLS_Y_SY, Styles::Operation }, 			// "+Y"
    {token_type::MIN_Y_SY, Styles::Operation }, 			// "-Y"
    {token_type::TIMES_Y_SY, Styles::Operation }, 		// "*Y"
    {token_type::DIV_Y_SY, Styles::Operation }, 			// "/Y"

    /* Car */
    {token_type::ASSIGN_CAR_SY, Styles::Car }, 		// "CAR:="
    {token_type::CAR_PLS_PLS_SY, Styles::Car }, 	// "CAR++"
    {token_type::MASKE_SY, Styles::Car }, 			// "MASKE"
    {token_type::_4COP_SY, Styles::Car }, 			// "4COP"
    {token_type::IF_SY, Styles::Car },    			// "IF"
    {token_type::EQ_SY, Styles::Car },   			// "="

    /* Comment */
    {token_type::COMMENT_SY, Styles::Comment},

    /* Error */
    {token_type::UNKNOWN, Styles::Error}
};


    int setStyleForToken(const token_type & token);

    std::string m_source;


};

#endif //MICROSIMULATION_MICROCODELEXER_H