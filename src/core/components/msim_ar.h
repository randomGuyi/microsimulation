/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */
#ifndef MSIM_AR_H
#define MSIM_AR_H


#include "core/components/msim_component.h"
#include <string>

 namespace core::components {
     class msim_ar final : public msim_component{
     public:
         msim_ar(std::string const & id, std::string const & label);
         void reset() override{}

     };
 }
#endif // MSIM_AR_H
