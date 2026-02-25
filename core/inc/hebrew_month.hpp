#pragma once

#include <string>
#include <iostream>

namespace calendrical {
    /**
     * @brief Hebrew calendar month enumeration.
     *
     * Values follow the numbering used by the library's Hebrew calendar
     * algorithms, with `adarii` representing Adar II in leap years.
     */
    enum class hebrew_month {
        /** @brief Nisan (month 1 in ecclesiastical numbering). */
        nisan = 1,
        /** @brief Iyyar. */
        iyyar,
        /** @brief Sivan. */
        sivan,
        /** @brief Tammuz. */
        tammuz,
        /** @brief Av. */
        av,
        /** @brief Elul. */
        elul,
        /** @brief Tishri (civil year starts here in many contexts). */
        tishri,
        /** @brief Marheshvan (Cheshvan). */
        marheshvan,
        /** @brief Kislev. */
        kislev,
        /** @brief Tevet. */
        tevet,
        /** @brief Shevat. */
        shevat,
        /** @brief Adar (or Adar I in leap years, depending on context). */
        adar,
        /** @brief Adar II (used in Hebrew leap years). */
        adarii
    };

    /** @brief Returns the lowercase transliterated Hebrew month name. */
    std::string to_string(const hebrew_month& m);

}

/** @brief Stream output for @ref calendrical::hebrew_month. */
std::ostream& operator<<(std::ostream& out, const calendrical::hebrew_month& m);
