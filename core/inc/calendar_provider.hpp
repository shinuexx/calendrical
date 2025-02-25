#pragma once

#include <map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include "fixed_date.hpp"
#include "types.hpp"

namespace calendrical{
    class calendar_provider{
        std::map<std::type_index, std::map<fixed_date_t, std::unique_ptr<const calendar_if>>> m_calendars;

    public:
        calendar_provider() : m_calendars() {}

        [[nodiscard]] const calendar_if* get(const std::type_index& type, const fixed_date_t& epoch) const;
        [[nodiscard]] const calendar_if* get(const std::type_index& type) const;
        const calendar_if* set(const std::type_index& type, const fixed_date_t& epoch, const calendar_if* calendar);
        const calendar_if* set(const std::type_index& type, const calendar_if* calendar);

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<calendar_if, T>>>
        [[nodiscard]] const T* get(const fixed_date_t& epoch) const{
            return dynamic_cast<const T*>(get(typeid(T), epoch));
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<calendar_if, T>>>
        [[nodiscard]] const T * get() const {
            return dynamic_cast<const T*>(get(typeid(T)));
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<calendar_if, T>>>
        const T* set(const fixed_date_t& epoch, const T * calendar){
            return dynamic_cast<const T*>(set(typeid(T), epoch, calendar));
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<calendar_if, T>>>
        const T* set(const T* calendar){
            return dynamic_cast<const T*>(set(typeid(T), calendar));
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<calendar_if, T>>>
        [[nodiscard]] const T* create(){
            return set(new T());
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<calendar_if, T>>>
        [[nodiscard]] const T* create(const fixed_date_t& epoch){
            return set(new T(epoch));
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<calendar_if, T>>>
        [[nodiscard]] const T* get_or_create(const fixed_date_t& epoch){
            auto calendar = get<T>(epoch);
            return calendar ? calendar : create<T>(epoch);
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<calendar_if, T>>>
        [[nodiscard]] const T* get_or_create(){
            auto calendar = get<T>();
            return calendar ? calendar : create<T>();
        }
    };
}