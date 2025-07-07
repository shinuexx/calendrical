#pragma once

#include <concepts>
#include <cstdint>
#include <vector>

namespace calendrical{
    template<typename T>
    concept has_year = requires(T value){
        { value.year } ->std::convertible_to<int32_t>;
    };

    template<typename T, typename MT>
    concept has_month = requires(T value){
        { value.month } -> std::convertible_to<MT>;
    };

    template<typename T>
    concept has_day = requires(T value){
        { value.day } ->std::convertible_to<int32_t>;
    };

    template<typename T>
    concept has_week = requires(T value){
        { value.week } ->std::convertible_to<int32_t>;
    };

    template<typename T, typename MT>
    concept has_leap_month = has_month<T, MT> && requires(T value){
        { value.leap_month } ->std::convertible_to<bool>;
    };

    template<typename T>
    concept has_leap_day = has_day<T> && requires(T value){
        { value.leap_day } ->std::convertible_to<bool>;
    };

    template<typename T>
    concept has_cycle = requires(T value){
        { value.cycle } ->std::convertible_to<int32_t>;
    };

    template<typename T>
    concept has_major = requires(T value){
        { value.major } ->std::convertible_to<int32_t>;
    };

    template<typename T, typename MT, typename FD>
    concept month_aware_calendar = requires(const T& value, const FD& date){
        { value.get_month(date) } ->std::convertible_to<int32_t>;
    };

    template<typename T, typename MT, typename FD>
    concept leap_month_aware_calendar = month_aware_calendar<T,MT,FD> && requires(const T& value, const FD& date){
        { value.is_leap_month(date) } -> std::convertible_to<bool>;
    };

    template<typename T, typename FD>
    concept week_aware_calendar = requires(const T& value, const FD& date){
        { value.get_week(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename FD>
    concept day_aware_calendar = requires(const T& value, const FD& date){
        { value.get_day(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename FD>
    concept leap_day_aware_calendar = day_aware_calendar<T,FD> && requires(const T& value, const FD& date){
        { value.is_leap_day(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename FD>
    concept year_aware_calendar = requires(const T& value, const FD& date){
        { value.get_year(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename FD>
    concept new_year_aware_calendar = requires(const T& value, const FD& date){
        { value.get_new_year(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename FD>
    concept is_leap_year_aware_calendar = requires(const T& value, int32_t year){
        { value.is_leap_year(year) } -> std::convertible_to<bool>;
    };

    template<typename T, typename FD>
    concept major_aware_calendar = requires(const T& value, const FD& date){
        { value.get_major(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename FD>
    concept cycle_aware_calendar = requires(const T& value, const FD& date){
        { value.get_cycle(date) } -> std::convertible_to<int32_t>;
    };

    template<typename T, typename MT, typename R>
    concept in_gregorian_aware_provider = requires(const T& value, int32_t year, MT month, int32_t day) {
        { value.in_gregorian(month, day, year) } -> std::ranges::range;
    };
}