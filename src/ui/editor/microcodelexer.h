/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */


#ifndef MICROSIMULATION_MICROCODELEXER_H
#define MICROSIMULATION_MICROCODELEXER_H

#include "../../core/parser/msim_scanner.h"

#include <Qsci/qscilexercustom.h>
#include <map>
namespace gui::views {

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
        std::map<core::parser::token_type, Styles> token_to_syle = {

            /* default */
            {core::parser::token_type::COND_NE0_SY, Styles::Default }, 		// "cond<>0"
            {core::parser::token_type::WRITE_SY, Styles::Default },   		// "write"
            {core::parser::token_type::READ_SY, Styles::Default },  			// "read"
            {core::parser::token_type::WAIT_SY, Styles::Default },     		// "wait"
            {core::parser::token_type::RAM_SY, Styles::Default }, 			// "RAM:"
            {core::parser::token_type::H_SY, Styles::Default }, 				// "H:"
            {core::parser::token_type::B_SY, Styles::Default }, 				// "B:"
            {core::parser::token_type::R_SY, Styles::Default }, 				// "R:"
            {core::parser::token_type::SEMICOLON_SY, Styles::Default },  		// "\n"
            {core::parser::token_type::COMMA_SY, Styles::Default }, 			// ","

            /* number */
            {core::parser::token_type::HASH_SY,Styles::Number },  			// "#"
            {core::parser::token_type::SLASH_SY,Styles::Number }, 			// "/"
            {core::parser::token_type::NUMBER_SY,Styles::Number },

            /* register */
            {core::parser::token_type::MDR_TO_COP_SY, Styles::Register }, 		// "MDR->COP"
            {core::parser::token_type::MDR_TO_Y_SY, Styles::Register }, 		// "MDR->Y"
            {core::parser::token_type::R0_TO_X_SY, Styles::Register }, 		// "R0->X"
            {core::parser::token_type::R1_TO_X_SY, Styles::Register }, 		// "R1->X"
            {core::parser::token_type::R2_TO_X_SY, Styles::Register }, 		// "R2->X"
            {core::parser::token_type::R3_TO_X_SY, Styles::Register }, 		// "R3->X"
            {core::parser::token_type::R0_TO_Y_SY, Styles::Register }, 		// "R0->Y"
            {core::parser::token_type::R1_TO_Y_SY, Styles::Register }, 		// "R1->Y"
            {core::parser::token_type::R2_TO_Y_SY, Styles::Register }, 		// "R2->Y"
            {core::parser::token_type::R3_TO_Y_SY, Styles::Register }, 		// "R3->Y"

            /* bus */
            {core::parser::token_type::Z_MAR_SY, Styles::Bus }, 			// "Z->MAR"
            {core::parser::token_type::Z_MDR_SY, Styles::Bus }, 		    // "Z->MDR"
            {core::parser::token_type::Z_R0_SY, Styles::Bus }, 			// "Z->R0"
            {core::parser::token_type::Z_R1_SY, Styles::Bus }, 			// "Z->R1"
            {core::parser::token_type::Z_R2_SY, Styles::Bus },  			// "Z->R2"
            {core::parser::token_type::Z_R3_SY, Styles::Bus }, 			// "Z->R3"
            {core::parser::token_type::Z_SY, Styles::Bus }, 				// "Z"
            {core::parser::token_type::X_SY, Styles::Bus }, 				// "X"
            {core::parser::token_type::Y_SY, Styles::Bus }, 				// "Y"

            /* Operation */
            {core::parser::token_type::INC_SY, Styles::Operation }, 			// "INC"
            {core::parser::token_type::DEC_SY, Styles::Operation }, 			// "DEC"
            {core::parser::token_type::SHL_Y_SY, Styles::Operation }, 			// "<<Y"
            {core::parser::token_type::AR_SY, Styles::Operation }, 				// "AR:"
            {core::parser::token_type::_4_CN_SY, Styles::Operation }, 			// "4CN"
            {core::parser::token_type::CN_ASSIGN_SY, Styles::Operation },   	// "CN="
            {core::parser::token_type::Z_ASSIGN_SY, Styles::Operation }, 		// "Z:="
            {core::parser::token_type::MIN_X_SY, Styles::Operation }, 			// "-X"
            {core::parser::token_type::PLS_Y_SY, Styles::Operation }, 			// "+Y"
            {core::parser::token_type::MIN_Y_SY, Styles::Operation }, 			// "-Y"
            {core::parser::token_type::TIMES_Y_SY, Styles::Operation }, 		// "*Y"
            {core::parser::token_type::DIV_Y_SY, Styles::Operation }, 			// "/Y"

            /* Car */
            {core::parser::token_type::ASSIGN_CAR_SY, Styles::Car }, 		// "CAR:="
            {core::parser::token_type::CAR_PLS_PLS_SY, Styles::Car }, 	// "CAR++"
            {core::parser::token_type::MASKE_SY, Styles::Car }, 			// "MASKE"
            {core::parser::token_type::_4COP_SY, Styles::Car }, 			// "4COP"
            {core::parser::token_type::IF_SY, Styles::Car },    			// "IF"
            {core::parser::token_type::EQ_SY, Styles::Car },   			// "="

            /* Comment */
            {core::parser::token_type::COMMENT_SY, Styles::Comment},

            /* Error */
            {core::parser::token_type::UNKNOWN, Styles::Error}
        };


        int setStyleForToken(const core::parser::token_type & token);

        std::string m_source;


    };
}
#endif //MICROSIMULATION_MICROCODELEXER_H