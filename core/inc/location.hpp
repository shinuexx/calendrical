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
        [[nodiscard]] constexpr moment_t get_longitude_zone() const { return longitude / angle_t::full_circle; }

        /** @brief Initial bearing from this location to @p other. */
        [[nodiscard]] angle_t direction(const location& other) const;
    };

    namespace locations {
        /** @brief Urbana, Illinois, USA. */
        static constexpr location urbana{ 40.1, -88.2, 225, moment_t::hms(-6) };
        /** @brief Royal Observatory Greenwich, UK. */
        static constexpr location greenwich{ 51.4777815, 0, 46.9, 0 };
        /** @brief Mecca, Saudi Arabia. */
        static constexpr location mecca{ angle_t::dms(21,25,24), angle_t::dms(39,49,24), 298, moment_t::hms(3) };
        /** @brief Jerusalem, Israel. */
        static constexpr location jerusalem{ 31.78, 35.24, 740, moment_t::hms(2) };
        /** @brief Acre (Akko), Israel. */
        static constexpr location acre{ 32.94, 35.09, 740, moment_t::hms(2) };
        /** @brief CFS Alert, Nunavut, Canada. */
        static constexpr location cfs_alert{ angle_t::dms(82,30), -angle_t::dms(62,19), 0, moment_t::hms(-5) };
        /** @brief Padua, Italy. */
        static constexpr location padua{ angle_t::dms(45,24,28), angle_t::dms(11,53,9), 18, moment_t::hms(1) };
    }
}
