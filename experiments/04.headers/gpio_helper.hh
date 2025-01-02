#pragma once
#include <platform-gpio.hh>

/**
 * Override the & operator for the Joystick so that we can do comparisons.
 */
inline constexpr SonataJoystick
operator&(SonataJoystick x, SonataJoystick y)
{
	return static_cast<SonataJoystick>(static_cast<int>(x) & static_cast<int>(y));
}

/**
 * Override the | operator for the Joystick so that we can do comparisons.
 */
inline constexpr SonataJoystick
operator|(SonataJoystick x, SonataJoystick y)
{
	return static_cast<SonataJoystick>(static_cast<int>(x) | static_cast<int>(y));
}

/**
 * Override the ^ operator for the Joystick so that we can do comparisons.
 */
inline constexpr SonataJoystick
operator^(SonataJoystick x, SonataJoystick y)
{
	return static_cast<SonataJoystick>(static_cast<int>(x) ^ static_cast<int>(y));
}
  
/**
 * Override the ~ operator for the Joystick so that we can do comparisons.
 */
inline constexpr SonataJoystick
operator~(SonataJoystick x)
{
	return static_cast<SonataJoystick>(~ static_cast<int>(x));
}
