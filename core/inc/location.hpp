#pragma once

#include "moment.hpp"
#include "angle.hpp"

namespace calendrical {
    /**
     * @brief Geographic location parameters used by astronomical calculations.
     *
     * Coordinates are expressed in degrees (`angle_t`), elevation in meters, and
     * `zone` as a time offset stored in days (`moment_t`).
     */
    struct location {
        /** @brief Geographic latitude in degrees (north positive). */
        angle_t latitude;
        /** @brief Geographic longitude in degrees (east positive). */
        angle_t longitude;
        /** @brief Elevation above sea level in meters. */
        double elevation{};
        /** @brief Time zone offset from UTC, represented in days. */
        moment_t zone;

        /** @brief Longitude-derived mean solar time zone offset (longitude / 360). */
        [[nodiscard]] constexpr moment_t get_longitude_zone() const { return moment_t{ longitude / angle_t::full_circle }; }

        /** @brief Initial bearing from this location to @p other. */
        [[nodiscard]] angle_t direction(const location& other) const;
    };

    namespace locations {
        /** @brief Urbana, Illinois, USA. */
        inline constexpr location urbana{ 40.1_deg, -88.2_deg, 225, moment_t::hms(-6) };
        /** @brief Royal Observatory Greenwich, UK. */
        inline constexpr location greenwich{ 51.4777815_deg, 0_deg, 46.9, 0_m };
        /** @brief Mecca, Saudi Arabia. */
        inline constexpr location mecca{ angle_t::dms(21,25,24), angle_t::dms(39,49,24), 298, moment_t::hms(3) };
        /** @brief Jerusalem, Israel. */
        inline constexpr location jerusalem{ 31.78_deg, 35.24_deg, 740, moment_t::hms(2) };
        /** @brief Acre (Akko), Israel. */
        inline constexpr location acre{ 32.94_deg, 35.09_deg, 740, moment_t::hms(2) };
        /** @brief CFS Alert, Nunavut, Canada. */
        inline constexpr location cfs_alert{ angle_t::dms(82,30), -angle_t::dms(62,19), 0, moment_t::hms(-5) };
        /** @brief Padua, Italy. */
        inline constexpr location padua{ angle_t::dms(45,24,28), angle_t::dms(11,53,9), 18, moment_t::hms(1) };
    }
}
