#pragma once

#include <concepts>
#include <cstdint>
#include <vector>

namespace calendrical{
    template<typename T>
    /** @brief Type has a `year` data member convertible to `int32_t`. */
    concept has_year = requires(T value){
        { value.year } ->std::convertible_to<int32_t>;
    };

    template<typename T, typename MT>
    /** @brief Type has a `month` data member convertible to `MT`. */
    concept has_month = requires(T value){
        { value.month } -> std::convertible_to<MT>;
    };

    template<typename T>
    /** @brief Type has a `day` data member convertible to `int32_t`. */
    concept has_day = requires(T value){
        { value.day } ->std::convertible_to<int32_t>;
    };

    template<typename T>
    /** @brief Type has a `week` data member convertible to `int32_t`. */
    concept has_week = requires(T value){
        { value.week } ->std::convertible_to<int32_t>;
    };

    template<typename T, typename MT>
    /** @brief Type has `month` plus boolean `leap_month` marker. */
    concept has_leap_month = has_month<T, MT> && requires(T value){
        { value.leap_month } ->std::convertible_to<bool>;
    };

    template<typename T>
    /** @brief Type has `day` plus boolean `leap_day` marker. */
    concept has_leap_day = has_day<T> && requires(T value){
        { value.leap_day } ->std::convertible_to<bool>;
    };

    template<typename T>
    /** @brief Type has a `cycle` data member convertible to `int32_t`. */
    concept has_cycle = requires(T value){
        { value.cycle } ->std::convertible_to<int32_t>;
    };

    template<typename T>
    /** @brief Type has a `major` data member convertible to `int32_t`. */
    concept has_major = requires(T value){
        { value.major } ->std::convertible_to<int32_t>;
    };

    template<typename T, typename MT, typename FD>
    /** @brief Calendar exposes `get_month(FD)` returning a month-like value. */
    concept month_aware_calendar = requires(const T& value, const FD& date){
        { value.get_month(date) } ->std::convertible_to<MT>;
    };

    template<typename T, typename MT, typename FD>
    /** @brief Calendar exposes `get_month` plus `is_leap_month`. */
    concept leap_month_aware_calendar = month_aware_calendar<T,MT,FD> && requires(const T& value, const FD& date){
        { value.is_leap_month(date) } -> std::convertible_to<bool>;
    };

    template<typename T, typename FD>
    /** @brief Calendar exposes `get_week(FD)`. */
    concept week_aware_calendar = requires(const T& value, const FD& date){
        { value.get_week(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename FD>
    /** @brief Calendar exposes `get_day(FD)`. */
    concept day_aware_calendar = requires(const T& value, const FD& date){
        { value.get_day(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename FD>
    /** @brief Calendar exposes `get_day` plus `is_leap_day`. */
    concept leap_day_aware_calendar = day_aware_calendar<T,FD> && requires(const T& value, const FD& date){
        { value.is_leap_day(date) } -> std::convertible_to<bool>;
    };

    template<typename T, typename FD>
    /** @brief Calendar exposes `get_year(FD)`. */
    concept year_aware_calendar = requires(const T& value, const FD& date){
        { value.get_year(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename FD>
    /** @brief Calendar exposes `get_new_year(year)` returning `FD`. */
    concept new_year_aware_calendar = requires(int32_t year, const T& value, const FD& date){
        { value.get_new_year(year) } -> std::convertible_to<FD>;
    };

    template<typename T, typename FD>
    /** @brief Calendar exposes `is_leap_year(year)`. */
    concept is_leap_year_aware_calendar = requires(const T& value, int32_t year){
        { value.is_leap_year(year) } -> std::convertible_to<bool>;
    };

    template<typename T, typename FD>
    /** @brief Calendar exposes `get_major(FD)`. */
    concept major_aware_calendar = requires(const T& value, const FD& date){
        { value.get_major(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename FD>
    /** @brief Calendar exposes `get_cycle(FD)`. */
    concept cycle_aware_calendar = requires(const T& value, const FD& date){
        { value.get_cycle(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename MT, typename R>
    /** @brief Provider exposes `in_gregorian(month, day, year)` returning a range. */
    concept in_gregorian_aware_provider = requires(const T& value, int32_t year, MT month, int32_t day) {
        { value.in_gregorian(month, day, year) } -> std::ranges::range;
    };
}
