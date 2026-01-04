#ifndef MSIM_AR_H
#define MSIM_AR_H


#include "core/components/msim_component.h"
#include <string>

 namespace core::components {
     class msim_ar : public msim_component{
     public:
         msim_ar(std::string const & id, std::string const & label);
         void reset() override{}

     private:
     };
 }
#endif // MSIM_AR_H
