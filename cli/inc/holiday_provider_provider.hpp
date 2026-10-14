#pragma once

#include <concepts>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "types.hpp"

namespace calendrical::cli {
    /** @brief Registry of holiday providers, looked up by name. */
    class holiday_provider_provider final {
        /** @brief Holiday providers keyed by name. */
        std::map<std::string, std::unique_ptr<holiday_provider_if>, std::less<>> m_holiday_providers;

    public:
        /** @brief Creates an empty registry. */
        holiday_provider_provider() = default;

        /** @brief Returns the provider registered as @p name, or `nullptr`. */
        const holiday_provider_if* operator[](std::string_view name) const;
        /**
         * @brief Registers @p provider as @p name.
         *
         * A null @p provider is ignored; a name that is already taken is reported on stderr and ignored.
         */
        holiday_provider_provider& add(std::string name, std::unique_ptr<holiday_provider_if> provider);

        /** @brief Constructs a @p HolidayProviderT from @p args and registers it as @p name. */
        template<std::derived_from<holiday_provider_if> HolidayProviderT, typename... Args>
        holiday_provider_provider& add(std::string name, Args&&... args) {
            return add(std::move(name), std::make_unique<HolidayProviderT>(std::forward<Args>(args)...));
        }

        /** @brief All registered providers, ordered by name. */
        [[nodiscard]] const auto& holiday_providers() const { return m_holiday_providers; }
    };
}
