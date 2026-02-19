#pragma once

#include <concepts>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "types.hpp"

namespace calendrical::cli {
    class holiday_provider_provider final {
        std::map<std::string, std::unique_ptr<holiday_provider_if>, std::less<>> m_holiday_providers;

    public:
        holiday_provider_provider() = default;

        const holiday_provider_if* operator[](std::string_view name) const;
        holiday_provider_provider& add(std::string name, std::unique_ptr<holiday_provider_if> provider);

        template<std::derived_from<holiday_provider_if> HolidayProviderT, typename... Args>
        holiday_provider_provider& add(std::string name, Args&&... args) {
            return add(std::move(name), std::make_unique<HolidayProviderT>(std::forward<Args>(args)...));
        }

        [[nodiscard]] const auto& holiday_providers() const { return m_holiday_providers; }
    };
}
