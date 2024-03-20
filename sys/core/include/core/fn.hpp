//
// Created by chk on 3/17/2024.
//

#ifndef CHK_PIX_CORE_FN_HPP
#define CHK_PIX_CORE_FN_HPP

#include <functional>

template <typename T> using Fn = std::function<T>;
using Callback = Fn<void()>;

#endif // CHK_PIX_CORE_FN_HPP
