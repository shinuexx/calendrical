#pragma once

#include <variant>
#include <functional>

namespace calendrical{
    /**
     * @brief Lazily initialized value wrapper.
     *
     * Stores either a deferred factory (`std::function<T()>`) or an already
     * materialized value and computes the value on first access.
     */
    template<typename T>
    class lazy{
        mutable std::variant<std::function<T()>, T> m_data;

    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        template<typename = std::enable_if_t<std::is_default_constructible_v<T>>>
        /** @brief Creates a loaded lazy object with `T{}`. */
        explicit lazy() : m_data{T{}}{}
        template<typename = std::enable_if_t<std::is_copy_constructible_v<T>>>
        /** @brief Creates a loaded lazy object from a copied value. */
        explicit lazy(const T& other) : m_data{other}{}
        template<typename = std::enable_if_t<std::is_move_constructible_v<T>>>
        /** @brief Creates a loaded lazy object from a moved value. */
        explicit lazy(T&& other) : m_data{std::move(other)}{}

        /** @brief Creates a deferred lazy object from a copyable factory. */
        explicit lazy(const std::function<T()> &data) : m_data(data) {}
        /** @brief Creates a deferred lazy object from a movable factory. */
        explicit lazy(std::function<T()> &&data) : m_data(std::move(data)) {}

        /** @brief Returns true if the value has already been materialized. */
        [[nodiscard]] bool loaded() const { return m_data.index() == 1; }

        /** @brief Forces evaluation (if needed) and returns the stored value. */
        reference load() const {
            if (!loaded())
                m_data = std::invoke(std::get<0>(m_data));
            return std::get<1>(m_data);
        }

        /** @brief Explicit conversion to a mutable reference of the value. */
        explicit operator reference() const { return load(); }
        /** @brief Explicit conversion to a copied value. */
        explicit operator value_type() const { return load(); }
        /** @brief Dereferences to the materialized value. */
        reference operator*() const { return load(); }
        /** @brief Member access to the materialized value. */
        pointer operator->() const { return std::addressof(load()); }
    };

    template<typename T>
    /** @brief Creates a deferred lazy object that default-constructs on first access. */
    lazy<T> make_deferred_default_lazy() {
        return lazy<T>([]() -> T { return T(); });
    }
}
