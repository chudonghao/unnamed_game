//
// Created by chudonghao on 17-12-23.
//

#ifndef UNNAMED_GAME_ID_SHARED_PTR_MAP_H
#define UNNAMED_GAME_ID_SHARED_PTR_MAP_H

#include <memory>
#include <map>
#include "log.h"
#include "has_id.h"

namespace unnamed_game {

//template<typename T, typename enable_t = void>
//class id_shared_ptr_map1_t {};
//
//template<typename T>
//class id_shared_ptr_map1_t<T, typename std::enable_if<std::is_base_of<has_id_t, T>::value >::type>
//        : public std::map<std::string/*id*/, std::shared_ptr<T>> {
//public:
//    template<typename... args_t>
//    std::shared_ptr<T> &create(const std::string &id, args_t &&...args);
//};
//
//TODO why?? : invalid use of incomplete type
//template<typename T>
//template<typename... args_t>
//std::shared_ptr<T> &id_shared_ptr_map1_t<T, void>::create(const std::string &id, args_t &&... args) {
//    auto iter = id_shared_ptr_map1_t::find(id);
//    if (iter == id_shared_ptr_map1_t::end()) {
//        auto &result = id_shared_ptr_map1_t::operator[](id);
//        result = std::make_shared<T>(std::forward<args_t>(args)...);
//        result->id = id;
//        return result;
//    } else {
//        LOG_W << "same id object (id = " << id << ")";
//        return iter->second;
//    }
//}


template<typename T>
class id_shared_ptr_map_t : public std::map<std::string/*id*/, std::shared_ptr<T>> {
    typedef typename std::enable_if<std::is_base_of<has_id_t, T>::value, std::shared_ptr<T>>::type unnamed;
public:
    template<typename... args_t>
    std::shared_ptr<T> &
    create(const std::string &id, args_t &&... args);

    inline typename std::map<std::string/*id*/, std::shared_ptr<T>>::size_type destory(const std::string &id);

    inline void add(const std::string &id, std::shared_ptr<T> &shared_ptr);

    inline void remove(const std::string &id);
};

template<typename T>
template<typename... args_t>
std::shared_ptr<T> &id_shared_ptr_map_t<T>::create(const std::string &id, args_t &&... args) {
    auto iter = id_shared_ptr_map_t::find(id);
    if (iter == id_shared_ptr_map_t::end()) {
        auto &result = id_shared_ptr_map_t::operator[](id);
        result = std::make_shared<T>(std::forward<args_t>(args)...);
        result->id = id;
        return result;
    } else {
        LOG_W << "data(id=" << id << ") was already created,abandon creating.";
        return iter->second;
    }
}

template<typename T>
typename std::map<std::string/*id*/, std::shared_ptr<T>>::size_type
id_shared_ptr_map_t<T>::destory(const std::string &id) {
    return id_shared_ptr_map_t::erase(id);
}

template<typename T>
void id_shared_ptr_map_t<T>::add(const std::string &id, std::shared_ptr<T> &shared_ptr) {
    auto iter = id_shared_ptr_map_t::find(id);
    if (iter == id_shared_ptr_map_t::end()) {
        id_shared_ptr_map_t::operator[](id) = shared_ptr;
        shared_ptr->id = id;
    } else {
        LOG_W << "data(id=" << id << ") was already added,abandon adding.";
    }
}

template<typename T>
void id_shared_ptr_map_t<T>::remove(const std::string &id) {
    id_shared_ptr_map_t::erase(id);
}

}

#endif //UNNAMED_GAME_ID_SHARED_PTR_MAP_H
