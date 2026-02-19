#include "converter_provider.hpp"

namespace calendrical::cli {

    const converter *converter_provider::operator[](std::string_view name) const {
        const auto it = m_converters.find(name);
        if (it == m_converters.end()) {
            return nullptr;
        }
        return it->second.get();
    }
    converter_provider &converter_provider::add(std::unique_ptr<converter> conv) {
        if (m_converters.contains(conv->name()))
            std::cerr << "Warning: conversion already exists: " << conv->name() << std::endl;
        else
            m_converters.emplace(conv->name(), std::move(conv));
        return *this;
    }
} // namespace calendrical::cli
