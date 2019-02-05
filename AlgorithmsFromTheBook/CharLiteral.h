#pragma once

inline constexpr unsigned char operator "" _uc(unsigned long long arg) noexcept
{
	return static_cast< unsigned char >(arg);
}

inline constexpr char operator "" _c(unsigned long long arg) noexcept
{
	return static_cast< char >(arg);
}