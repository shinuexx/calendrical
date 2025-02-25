#pragma once

#include <variant>
#include <functional>

namespace calendrical{
    template<typename T>
    class lazy{
        mutable std::variant<std::function<T()>, T> m_data;

    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        template<typename = std::enable_if_t<std::is_default_constructible_v<T>>>
        explicit lazy() : m_data{T{}}{}
        template<typename = std::enable_if_t<std::is_copy_constructible_v<T>>>
        explicit lazy(const T& other) : m_data{other}{}
        template<typename = std::enable_if_t<std::is_move_constructible_v<T>>>
        explicit lazy(T&& other) : m_data{std::move(other)}{}

        explicit lazy(const std::function<T()> &data) : m_data(data) {}
        explicit lazy(std::function<T()> &&data) : m_data(std::move(data)) {}

        [[nodiscard]] bool loaded() const { return m_data.index() == 1; }

        reference load() const {
            if (!loaded())
                m_data = std::invoke(std::get<0>(m_data));
            return std::get<1>(m_data);
        }

        explicit operator reference() const { return load(); }
        explicit operator value_type() const { return load(); }
        reference operator*() const { return load(); }
        pointer operator->() const { return std::addressof(load()); }
    };

    template<typename T>
    lazy<T> make_deferred_default_lazy() {
        return lazy<T>([]() -> T { return T(); });
    }
}