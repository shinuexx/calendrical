#include "holiday_provider_provider.hpp"

#include <iostream>

namespace calendrical::cli {
    const holiday_provider_if* holiday_provider_provider::operator[](std::string_view name) const {
        const auto it = m_holiday_providers.find(name);
        if (it == m_holiday_providers.end()) {
            return nullptr;
        }
        return it->second.get();
    }

    holiday_provider_provider& holiday_provider_provider::add(std::string name, std::unique_ptr<holiday_provider_if> provider) {
        if (!provider) {
            return *this;
        }

        if (m_holiday_providers.contains(name)) {
            std::cerr << "Warning: holiday provider already exists: " << name << std::endl;
        } else {
            m_holiday_providers.emplace(std::move(name), std::move(provider));
        }
        return *this;
    }
}
