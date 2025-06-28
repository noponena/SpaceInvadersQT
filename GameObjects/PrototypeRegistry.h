#pragma once

#include "GameObject.h"
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace GameObjects {
struct PrototypeKey {
    ObjectType type;
    std::string variant;
    bool operator==(const PrototypeKey& other) const { return type == other.type && variant == other.variant; }
};

/**
 * @brief Generic registry for object prototypes, supporting fast cloning by type.
 *
 * Intended for use with a base class having a virtual clone() method.
 * Typical use: storing prototypes of game objects for fast instantiation.
 *
 * @tparam TypeId The type used as the registry key (e.g., enum or string)
 * @tparam BaseType The base type for all prototypes (must have virtual std::unique_ptr<BaseType> clone() const)
 */
template <typename TypeId, typename BaseType>
class PrototypeRegistry
{
    static_assert(std::is_default_constructible_v<TypeId>,
                  "TypeId must be default-constructible");
    static_assert(std::is_copy_constructible_v<TypeId>,
                  "TypeId must be copy-constructible");

public:
    /**
     * @brief Register a prototype instance for a given type.
     *
     * The registry will take ownership of the prototype.
     * Typically, you pass a std::make_unique<ConcreteType>(...) for each type.
     *
     * @param id The type identifier.
     * @param prototype The prototype object (ownership is transferred).
     * @throws std::runtime_error If a prototype for the given id already exists.
     */
    void registerPrototype(TypeId id, std::unique_ptr<BaseType> prototype)
    {
        if (!prototype)
            throw std::invalid_argument("Cannot register null prototype");
        auto [it, inserted] = m_registry.emplace(std::move(id), std::move(prototype));
        if (!inserted)
            throw std::runtime_error("Prototype already registered for this TypeId");
    }

    /**
     * @brief Clone the prototype for the given type.
     *
     * Returns a new instance cloned from the registered prototype.
     *
     * @param id The type identifier.
     * @return std::unique_ptr<BaseType> Newly cloned object.
     * @throws std::out_of_range If the id is not registered.
     */
    std::unique_ptr<BaseType> clone(TypeId id) const
    {
        auto it = m_registry.find(id);
        if (it == m_registry.end())
            throw std::out_of_range("Prototype not found for given TypeId");
        return it->second->clone();
    }

    /**
     * @brief Check if a prototype is registered for a type id.
     *
     * @param id The type identifier.
     * @return true If a prototype is registered.
     * @return false Otherwise.
     */
    bool hasPrototype(TypeId id) const
    {
        return m_registry.find(id) != m_registry.end();
    }

    /**
     * @brief Remove the prototype for the given type.
     *
     * Does nothing if the prototype is not found.
     *
     * @param id The type identifier.
     */
    void unregisterPrototype(TypeId id)
    {
        m_registry.erase(id);
    }

    /**
     * @brief Remove all registered prototypes.
     */
    void clear()
    {
        m_registry.clear();
    }

private:
    std::unordered_map<TypeId, std::unique_ptr<BaseType>> m_registry;
};

}
