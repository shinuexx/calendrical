#include "calendar_provider.hpp"

namespace calendrical{
    const calendar_if* calendar_provider::get(const std::type_index& type, const fixed_date_t& epoch) const{
        if (const auto it = m_calendars.find(type); it != m_calendars.end()){
            auto &r = it->second;
            if (const auto sit = r.find(epoch); sit != r.end()){
                return sit->second.get();
            }
        }
        return nullptr;
    }
    const calendar_if* calendar_provider::get(const std::type_index& type) const{
        if (const auto it = m_calendars.find(type); it != m_calendars.end()) {
            const auto r = it->second.begin();
            return r->second.get();
        }
        return nullptr;
    }
    const calendar_if* calendar_provider::set(const std::type_index& type, const fixed_date_t& epoch, const calendar_if* calendar){
        auto &calendars = m_calendars[type];
        auto &a = calendars[epoch];
        a.reset(calendar);
        return a.get();
    }
    const calendar_if* calendar_provider::set(const std::type_index& type, const calendar_if* calendar) {
        auto& calendars = m_calendars[type];
        calendars[calendar->epoch()].reset(calendar);
        return calendar;
    }
}