//
// Created by chk on 3/17/2024.
//

#ifndef CHK_PIX_CORE_HPP
#define CHK_PIX_CORE_HPP

#include <cstdint>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

using r32 = float;
using r64 = double;

using b32 = s32;

#include <string>
#include <string_view>
using String = std::string;
using StringW = std::wstring;
using StringView = std::string_view;
using StringWView = std::wstring_view;

#endif // CHK_PIX_CORE_HPP
