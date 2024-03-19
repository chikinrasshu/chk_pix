//
// Created by Jorge Botarro on 19-03-24.
//

#ifndef CHK_PIX_MAP_HPP
#define CHK_PIX_MAP_HPP

#include "core.hpp"

#include <unordered_map>

template<typename K, typename V> using Map = std::unordered_map<K, V>;
template<typename T> using StringMap = std::unordered_map<StringView, T>;

#endif //CHK_PIX_MAP_HPP
