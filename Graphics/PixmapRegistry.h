#ifndef PIXMAPREGISTRY_H
#define PIXMAPREGISTRY_H

#include <vector>
#include <functional>

class PixmapRegistry {
public:
    static PixmapRegistry& instance() {
        static PixmapRegistry reg;
        return reg;
    }
    void add(const std::function<void()>& func) {
        m_funcs.push_back(func);
    }
    void preloadAll() {
        for (auto& f : m_funcs)
            f();
    }
private:
    std::vector<std::function<void()>> m_funcs;
};

#endif // PIXMAPREGISTRY_H
