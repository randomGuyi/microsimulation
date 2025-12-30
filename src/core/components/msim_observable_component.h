#ifndef MSIM_OBSERVABLE_COMPONENT_H
#define MSIM_OBSERVABLE_COMPONENT_H

#include <functional>
namespace core::components {
    template<typename T>
    class msim_observable_component{
    public:
        using Callback = std::function<void(const T &)>;

        void subscibe(Callback cb){
            m_callbacks.push_back(std::move(cb));
        }

    protected:
        void notify(const T & value){
            for(auto & cb : m_callbacks){
                if(cb){
                    cb(value);
                }
            }
        }

    private:
        std::vector<Callback> m_callbacks;

    };
}
#endif // MSIM_OBSERVABLE_COMPONENT_H
