#pragma once

#include <functional>
#include <mutex>
#include <optional>
#include <stdexcept>

namespace calendrical{
    /**
     * @brief Lazily initialized value wrapper.
     *
     * Stores either a deferred factory (`std::function<T()>`) or an already
     * materialized value and computes the value on first access.
     *
     * First initialization is thread-safe and performed at most once.
     * The wrapper is intentionally non-copyable and non-movable.
     */
    template<typename T>
    class lazy{
        mutable std::optional<T> m_value;
        mutable std::function<T()> m_factory;
        mutable std::once_flag m_once;

        lazy(const lazy&) = delete;
        lazy& operator=(const lazy&) = delete;
        lazy(lazy&&) = delete;
        lazy& operator=(lazy&&) = delete;

    public:
        /** @brief Stored value type. */
        using value_type = T;
        /** @brief Pointer to the stored value type. */
        using pointer = T*;
        /** @brief Reference to the stored value type. */
        using reference = T&;

        template<typename = std::enable_if_t<std::is_default_constructible_v<T>>>
        /** @brief Creates a loaded lazy object with `T{}`. */
        explicit lazy() : m_value{T{}}{}
        template<typename = std::enable_if_t<std::is_copy_constructible_v<T>>>
        /** @brief Creates a loaded lazy object from a copied value. */
        explicit lazy(const T& other) : m_value{other}{}
        template<typename = std::enable_if_t<std::is_move_constructible_v<T>>>
        /** @brief Creates a loaded lazy object from a moved value. */
        explicit lazy(T&& other) : m_value{std::move(other)}{}

        /** @brief Creates a deferred lazy object from a copyable factory. */
        explicit lazy(const std::function<T()> &data) : m_factory(data) {}
        /** @brief Creates a deferred lazy object from a movable factory. */
        explicit lazy(std::function<T()> &&data) : m_factory(std::move(data)) {}

        /**
         * @brief Forces evaluation (if needed) and returns the stored value.
         * Thread-safe, initialized at most once.
         * @throws std::logic_error if neither a materialized value nor a factory is available.
         */
        reference load() const {
            std::call_once(m_once, [this] {
                if (!m_value) {
                    if (!m_factory) {
                        throw std::logic_error("lazy is not initialized: missing value and factory");
                    }
                    m_value = std::invoke(m_factory);
                    m_factory = {};
                }
            });
            return *m_value;
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
