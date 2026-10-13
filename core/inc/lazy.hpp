#pragma once

#include <functional>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <type_traits>

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

        void ensure_loaded() const {
            std::call_once(m_once, [this] {
                if (!m_value) {
                    if (!m_factory) {
                        throw std::logic_error("lazy is not initialized: missing value and factory");
                    }
                    m_value = std::invoke(m_factory);
                    m_factory = {};
                }
            });
        }

    public:
        /** @brief Stored value type. */
        using value_type = T;
        /** @brief Pointer to the stored value type. */
        using pointer = T*;
        /** @brief Const pointer to the stored value type. */
        using const_pointer = const T*;
        /** @brief Reference to the stored value type. */
        using reference = T&;
        /** @brief Const reference to the stored value type. */
        using const_reference = const T&;

        /** @brief Creates a loaded lazy object with `T{}`. */
        explicit lazy() requires std::is_default_constructible_v<T> : m_value{T{}}{}
        /** @brief Creates a loaded lazy object from a copied value. */
        explicit lazy(const T& other) requires std::is_copy_constructible_v<T> : m_value{other}{}
        /** @brief Creates a loaded lazy object from a moved value. */
        explicit lazy(T&& other) requires std::is_move_constructible_v<T> : m_value{std::move(other)}{}

        /** @brief Creates a deferred lazy object from a copyable factory. */
        explicit lazy(const std::function<T()> &data) : m_factory(data) {}
        /** @brief Creates a deferred lazy object from a movable factory. */
        explicit lazy(std::function<T()> &&data) : m_factory(std::move(data)) {}

        /**
         * @brief Forces evaluation (if needed) and returns a mutable reference to the stored value.
         * Thread-safe, initialized at most once.
         * @throws std::logic_error if neither a materialized value nor a factory is available.
         */
        reference load() {
            ensure_loaded();
            return *m_value;
        }

        /**
         * @brief Forces evaluation (if needed) and returns a const reference to the stored value.
         * Thread-safe, initialized at most once.
         * @throws std::logic_error if neither a materialized value nor a factory is available.
         */
        const_reference load() const {
            ensure_loaded();
            return *m_value;
        }

        /** @brief Explicit conversion to a mutable reference of the value. */
        explicit operator reference() { return load(); }
        /** @brief Explicit conversion to a const reference of the value. */
        explicit operator const_reference() const { return load(); }
        /** @brief Explicit conversion to a copied value. */
        explicit operator value_type() const { return load(); }
        /** @brief Dereferences to the materialized value. */
        reference operator*() { return load(); }
        /** @brief Dereferences to the materialized value (const). */
        const_reference operator*() const { return load(); }
        /** @brief Member access to the materialized value. */
        pointer operator->() { return std::addressof(load()); }
        /** @brief Member access to the materialized value (const). */
        const_pointer operator->() const { return std::addressof(load()); }
    };

    template<typename T>
    /** @brief Creates a deferred lazy object that default-constructs on first access. */
    lazy<T> make_deferred_default_lazy() {
        return lazy<T>([]() -> T { return T(); });
    }
}
