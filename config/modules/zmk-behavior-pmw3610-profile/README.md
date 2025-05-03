# ZMK PMW3610 Profile Switching Behavior

This module adds a behavior for ZMK that allows on-the-fly switching between precision and speed profiles for the PMW3610 optical sensor.

## Usage

Add the behaviors to your keymap:

```dts
#include <behaviors/pmw3610_profile.dtsi>

/ {
    keymap {
        compatible = "zmk,keymap";
        default_layer {
            bindings = <
                // Add these to your keymap
                &pmw_prec    &pmw_speed
            >;
        };
    };
};
```

- `&pmw_prec` - Switches to precision profile (lower CPI, higher smoothing) while key is pressed
- `&pmw_speed` - Switches to speed profile (higher CPI, lower smoothing) while key is pressed

## Configuration

The profiles use the settings defined in your `CONFIG_PMW3610_PRECISION_PROFILE_CPI` and `CONFIG_PMW3610_SPEED_PROFILE_CPI` values.
If you're using the smoothing filter, it will also use `CONFIG_PMW3610_PRECISION_PROFILE_SMOOTHING` and
`CONFIG_PMW3610_SPEED_PROFILE_SMOOTHING` respectively.
