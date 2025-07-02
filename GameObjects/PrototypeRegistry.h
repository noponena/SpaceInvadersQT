#pragma once

#include "GameObject.h"
#include <QString>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>

// ProtypeKey serves as the key for various game object prototypes
struct PrototypeKey {
  GameObjects::ObjectType type;
  std::string variant;
  bool operator==(const PrototypeKey &other) const {
    return type == other.type && variant == other.variant;
  }
};

// Hash specialization for PrototypeKey
namespace std {
template <> struct hash<PrototypeKey> {
  std::size_t operator()(const PrototypeKey &k) const noexcept {
    std::size_t h1 =
        std::hash<std::underlying_type_t<GameObjects::ObjectType>>{}(
            static_cast<std::underlying_type_t<GameObjects::ObjectType>>(
                k.type));
    std::size_t h2 = std::hash<std::string>{}(k.variant);
    return h1 ^ (h2 << 1);
  }
};
} // namespace std

namespace GameObjects {

/**
 * @brief Generic registry for object prototypes, supporting fast cloning by
 * type.
 *
 * Intended for use with a base class having a virtual clone() method.
 * Typical use: storing prototypes of game objects for fast instantiation.
 *
 * @tparam TypeId The type used as the registry key (e.g., enum or string)
 * @tparam BaseType The base type for all prototypes (must have virtual
 * std::unique_ptr<BaseType> clone() const)
 */
template <typename TypeId, typename BaseType> class PrototypeRegistry {
  static_assert(std::is_default_constructible_v<TypeId>,
                "TypeId must be default-constructible");
  static_assert(std::is_copy_constructible_v<TypeId>,
                "TypeId must be copy-constructible");

public:
  static PrototypeRegistry &instance() {
    static PrototypeRegistry instance;
    return instance;
  }

  /**
   * @brief Register a prototype instance for a given type.
   *
   * The registry will take ownership of the prototype.
   * Typically, you pass a std::make_unique<ConcreteType>(...) for each type.
   *
   * @param key The type identifier.
   * @param prototype The prototype object (ownership is transferred).
   * @param ignoreIfExists If true, do not throw if key exists; log a warning
   * instead (default: false).
   * @throws std::runtime_error If a prototype for the given key already exists
   * and ignoreIfExists is false.
   */
  void registerPrototype(const PrototypeKey &key,
                         std::unique_ptr<BaseType> prototype,
                         bool ignoreIfExists = false) {
    if (!prototype)
      throw std::invalid_argument("Cannot register null prototype");

    auto [it, inserted] = m_registry.emplace(key, std::move(prototype));
    if (!inserted) {
      if (ignoreIfExists) {
        qWarning() << "[PrototypeRegistry] Prototype already registered for key"
                   << QString::fromStdString(key.variant)
                   << "- ignoring registration.";
        return;
      } else {
        throw std::runtime_error(
            std::string("[PrototypeRegistry] Prototype already registered for key '") +
            key.variant + "'");
      }
    }

    qDebug() << "[PrototypeRegistry] Prototype registered for key"
             << QString::fromStdString(key.variant);
  }

  /**
   * @brief Clone the prototype for the given type.
   *
   * Returns a new instance cloned from the registered prototype.
   *
   * @param key The type identifier.
   * @return std::unique_ptr<BaseType> Newly cloned object.
   * @throws std::out_of_range If the key is not registered.
   */
  std::unique_ptr<BaseType> clone(const PrototypeKey &key) const {
    auto it = m_registry.find(key);
    if (it == m_registry.end())
      throw std::out_of_range(std::string("[PrototypeRegistry] Prototype not found for key '") +
                              key.variant + "'");
    return it->second->clone();
  }

  template <typename T>
  std::unique_ptr<T> cloneAs(const PrototypeKey &key) const {
    auto basePtr = this->clone(key); // unique_ptr<BaseType>
    auto raw = dynamic_cast<T *>(basePtr.release());
    if (!raw)
      throw std::runtime_error("cloneAs: Type mismatch!");
    return std::unique_ptr<T>(raw);
  }

  /**
   * @brief Check if a prototype is registered for a type id.
   *
   * @param key The type identifier.
   * @return true If a prototype is registered.
   * @return false Otherwise.
   */
  bool hasPrototype(const PrototypeKey &key) const {
    return m_registry.find(key) != m_registry.end();
  }

  /**
   * @brief Remove the prototype for the given type.
   *
   * Does nothing if the prototype is not found.
   *
   * @param key The type identifier.
   */
  void unregisterPrototype(const PrototypeKey &key) { m_registry.erase(key); }

  /**
   * @brief Remove all registered prototypes.
   */
  void clear() { m_registry.clear(); }

private:
  PrototypeRegistry() = default;
  PrototypeRegistry(const PrototypeRegistry &) = delete;
  PrototypeRegistry &operator=(const PrototypeRegistry &) = delete;
  std::unordered_map<PrototypeKey, std::unique_ptr<BaseType>> m_registry;
};

} // namespace GameObjects
