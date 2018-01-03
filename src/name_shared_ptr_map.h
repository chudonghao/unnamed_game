//
// Created by chudonghao on 17-12-26.
//

#ifndef UNNAMED_GAME_NAME_SHARED_PTR_MAP_H
#define UNNAMED_GAME_NAME_SHARED_PTR_MAP_H

#include <map>
#include <memory>
#include "log.h"

namespace unnamed_game {

template<typename T>
class name_shared_ptr_map_t : public std::map<std::string/*name*/, std::shared_ptr<T>> {
public:
    template<typename... args_t>
    std::shared_ptr<T> &create(const std::string &name, args_t &&... args);

    inline typename std::map<std::string/*name*/, std::shared_ptr<T>>::size_type destory(const std::string &name);

    inline void add(const std::string &name, std::shared_ptr<T> &shared_ptr);

    inline void remove(const std::string &name);
};

template<typename T>
template<typename... args_t>
std::shared_ptr<T> &
name_shared_ptr_map_t<T>::create(const std::string &name, args_t &&... args) {
    auto iter = name_shared_ptr_map_t::find(name);
    if (iter == name_shared_ptr_map_t::end()) {
        auto &result = name_shared_ptr_map_t::operator[](name);
        result = std::make_shared<T>(std::forward<args_t>(args)...);
        return result;
    } else {
        LOG_W << "same name object (name = " << name << ")";
        return iter->second;
    }
}

template<typename T>
inline typename std::map<std::string/*name*/, std::shared_ptr<T>>::size_type
name_shared_ptr_map_t<T>::destory(const std::string &name) {
    return name_shared_ptr_map_t::erase(name);
}

template<typename T>
void name_shared_ptr_map_t<T>::add(const std::string &name, std::shared_ptr<T> &shared_ptr) {
    if (name_shared_ptr_map_t<T>::find(name) != name_shared_ptr_map_t<T>::end()) {
        LOG_W << "same name object (name = " << name << ")";
    } else {
        if (shared_ptr) {
            name_shared_ptr_map_t::operator[](name) = shared_ptr;
        } else {
            LOG_W << "add nullptr";
        }
    }
}

template<typename T>
void name_shared_ptr_map_t<T>::remove(const std::string &name) {
    name_shared_ptr_map_t::erase(name);
}

}

#endif //UNNAMED_GAME_NAME_SHARED_PTR_MAP_H
