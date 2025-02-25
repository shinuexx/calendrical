#pragma once

#include "moment.hpp"
#include "angle.hpp"

namespace calendrical {
    struct location {
        angle_t latitude;
        angle_t longitude;
        double elevation{};
        moment_t zone;

        [[nodiscard]] constexpr moment_t get_longitude_zone() const { return longitude / angle_t::full_circle; }

        [[nodiscard]] angle_t direction(const location& other) const;
    };

    struct rational_location {
        rational_angle_t latitude;
        rational_angle_t longitude;
        rational elevation;
        rational_moment_t zone;

        [[nodiscard]] rational_moment_t get_longitude_zone() const { return longitude / rational_angle_t::full_circle; }

        explicit operator location() const;
    };

    namespace locations {
        static constexpr location urbana{ 40.1, -88.2, 225, moment_t::hms(-6) };
        static constexpr location greenwich{ 51.4777815, 0, 46.9, 0 };
        static constexpr location mecca{ angle_t::dms(21,25,24), angle_t::dms(39,49,24), 298, moment_t::hms(3) };
        static constexpr location jerusalem{ 31.78, 35.24, 740, moment_t::hms(2) };
        static constexpr location acre{ 32.94, 35.09, 740, moment_t::hms(2) };
        static constexpr location cfs_alert{ angle_t::dms(82,30), -angle_t::dms(62,19), 0, moment_t::hms(-5) };
        static constexpr location padua{ angle_t::dms(45,24,28), angle_t::dms(11,53,9), 18, moment_t::hms(1) };
    }
}