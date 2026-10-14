#pragma once

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cstdint>
#include <optional>
#include "fixed_date.hpp"
#include "calendars/gregorian_calendar.hpp"

namespace tests {

class holiday_csv {
    std::vector<int32_t> m_years;
    std::unordered_map<std::string, std::vector<std::string>> m_data;
    std::unordered_map<std::string, std::string> m_aliases;

    static std::string trim(const std::string& str) {
        const auto start = str.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        const auto end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }

    void init_aliases() {
        m_aliases = {
            {"bahai_new_year", "Bah\xc3\xa1'\xc3\xad New Year"},
            {"birth_of_the_bab", "Birth of the Ba\xcc\x84" "b"},
            {"naw_ruz", "Feast of Naw-R\xc5\xabz"},
            {"feast_of_ridvan", "Feast of Ri\xe1\xb8\x8d" "va\xcc\x84n"},
            {"coptic_christmas", "Christmas (Coptic)"},
            {"orthodox_christmas", "Christmas (Orthodox)"},
            {"orthodox_easter", "Easter (Orthodox)"},
            {"astronomical_easter", "Easter (Astronomical)"},
            {"unlucky_fridays", "Friday the 13th (first)"},
            {"classical_passover_eve", "Passover Eve (Classical)"},
            {"passover", "Passover"},
            {"yom_kippur", "Yom Kippur"},
            {"purim", "Purim"},
            {"ta_anit_esther", "Ta'anit Esther"},
            {"tishah_be_av", "Tishah be-Av"},
            {"yom_ha_zikkaron", "Yom ha-Zikkaron"},
            {"sh_ela", "Sh'ela"},
            {"hanukkah", "Hanukkah (first day)"},
            {"nowruz", "Nowruz"},
            {"losar", "Losar"},
            {"chinese_new_year", "Chinese New Year"},
            {"qing_ming", "Q\xc4\xabngm\xc3\xadng"},
            {"dragon_festival", "Dragon Festival"},
            {"mawlid", "Mawlid"},
            {"diwali", "Diwali"},
            {"shiva", "Great Night of Shiva"},
            {"rama", "Birthday of Rama"},
            {"hindu_lunar_new_year", "Hindu Lunar New Year"},
            {"mesha_samkranti", "Mesha Sa\xe1\xb9\x83kra\xcc\x84nti (date)"},
            {"sacred_wednesday", "Sacred Wednesday (first)"},
            {"icelandic_summer", "Icelandic Summer"},
            {"icelandic_winter", "Icelandic Winter"},
            {"advent_sunday", "Advent Sunday"},
            {"christmas", "Christmas"},
            {"epiphany", "Epiphany"},
            {"easter", "Easter"},
            {"pentecost", "Pentecost"},
            {"birkath_ha_hama", "Birkath ha-Hama"}
        };
    }

    void load_csv() {
        const std::vector<std::string> candidate_paths = {
#ifdef CALENDRICAL_DATA_DIR
            std::string(CALENDRICAL_DATA_DIR) + "/all-holidays.csv",
#endif
            "all-holidays.csv",
            "../all-holidays.csv",
            "../../all-holidays.csv",
            "tests/all-holidays.csv",
            "tests/data/all-holidays.csv"
        };

        std::ifstream file;
        for (const auto& path : candidate_paths) {
            file.open(path);
            if (file.is_open()) {
                break;
            }
        }

        if (!file.is_open()) {
            throw std::runtime_error("Could not find all-holidays.csv in any candidate path");
        }

        std::string line;
        if (std::getline(file, line)) {
            // Strip UTF-8 BOM if present
            if (line.size() >= 3 &&
                static_cast<unsigned char>(line[0]) == 0xef &&
                static_cast<unsigned char>(line[1]) == 0xbb &&
                static_cast<unsigned char>(line[2]) == 0xbf) {
                line = line.substr(3);
            }
            std::stringstream ss(line);
            std::string token;
            std::getline(ss, token, ';'); // Holiday
            std::getline(ss, token, ';'); // Func
            while (std::getline(ss, token, ';')) {
                auto t = trim(token);
                if (!t.empty()) {
                    m_years.push_back(std::stoi(t));
                }
            }
        }

        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string holiday_name, func, val;
            if (!std::getline(ss, holiday_name, ';')) continue;
            if (!std::getline(ss, func, ';')) continue;
            holiday_name = trim(holiday_name);
            if (holiday_name.empty()) continue;

            std::vector<std::string> vals;
            size_t year_idx = 0;
            while (std::getline(ss, val, ';')) {
                auto v = trim(val);
                if (v.empty() || v == "none") {
                    vals.push_back("none");
                } else {
                    const auto slash_pos = v.find('/');
                    if (slash_pos != std::string::npos && year_idx < m_years.size()) {
                        const int32_t y = m_years[year_idx];
                        const int32_t m = std::stoi(v.substr(0, slash_pos));
                        const int32_t d = std::stoi(v.substr(slash_pos + 1));
                        std::ostringstream oss;
                        oss << std::setfill('0')
                            << std::setw(4) << y << '-'
                            << std::setw(2) << m << '-'
                            << std::setw(2) << d;
                        vals.push_back(oss.str());
                    } else {
                        vals.push_back(v);
                    }
                }
                ++year_idx;
            }
            m_data[holiday_name] = std::move(vals);
        }
    }

    holiday_csv() {
        init_aliases();
        load_csv();
    }

public:
    static const holiday_csv& instance() {
        static const holiday_csv s_instance;
        return s_instance;
    }

    [[nodiscard]] const std::vector<int32_t>& years() const noexcept {
        return m_years;
    }

    [[nodiscard]] const std::vector<std::string>& get(const std::string& key) const {
        auto alias_it = m_aliases.find(key);
        const std::string& actual_key = (alias_it != m_aliases.end()) ? alias_it->second : key;
        auto it = m_data.find(actual_key);
        if (it == m_data.end()) {
            throw std::out_of_range("Holiday not found in CSV: " + key + " (mapped to: " + actual_key + ")");
        }
        return it->second;
    }

    static std::string format_date(calendrical::fixed_date_t fd) {
        const auto d = calendrical::calendars::gregorian_calendar::standard.to_date(fd);
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(4) << d.year << '-'
            << std::setw(2) << static_cast<int32_t>(d.month) << '-'
            << std::setw(2) << d.day;
        return oss.str();
    }

    static std::string format_date(const std::vector<calendrical::fixed_date_t>& fds) {
        if (fds.empty()) {
            return "none";
        }
        return format_date(fds.front());
    }

    static std::string format_date(const std::optional<calendrical::fixed_date_t>& opt) {
        if (!opt.has_value()) {
            return "none";
        }
        return format_date(*opt);
    }

    template <typename Func>
    void verify_holiday(const std::string& key, Func&& func) const {
        const auto& expected = get(key);
        ASSERT_EQ(expected.size(), m_years.size());
        for (size_t i = 0; i < m_years.size(); ++i) {
            const auto actual = format_date(func(m_years[i]));
            EXPECT_EQ(actual, expected[i])
                << "Mismatch for " << key << " in year " << m_years[i];
        }
    }
};

} // namespace tests
