/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_pmw3610_profile

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>

#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <pmw3610.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

enum profile_type {
    PROFILE_TYPE_PRECISION,
    PROFILE_TYPE_SPEED,
};

struct behavior_pmw3610_profile_config {
    enum profile_type profile_type;
};

static int on_pmw3610_profile_binding_pressed(struct zmk_behavior_binding *binding,
                                             struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding_device(binding);
    const struct behavior_pmw3610_profile_config *cfg = dev->config;
    
    LOG_DBG("PMW3610 profile behavior pressed");
    
    switch (cfg->profile_type) {
        case PROFILE_TYPE_PRECISION:
            LOG_DBG("Activating precision profile");
            return pmw3610_activate_precision_profile(true);
        case PROFILE_TYPE_SPEED:
            LOG_DBG("Activating speed profile");
            return pmw3610_activate_speed_profile(true);
        default:
            LOG_ERR("Unknown profile type: %d", cfg->profile_type);
            return -EINVAL;
    }
}

static int on_pmw3610_profile_binding_released(struct zmk_behavior_binding *binding,
                                              struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding_device(binding);
    const struct behavior_pmw3610_profile_config *cfg = dev->config;
    
    LOG_DBG("PMW3610 profile behavior released");
    
    switch (cfg->profile_type) {
        case PROFILE_TYPE_PRECISION:
            LOG_DBG("Deactivating precision profile");
            return pmw3610_activate_precision_profile(false);
        case PROFILE_TYPE_SPEED:
            LOG_DBG("Deactivating speed profile");
            return pmw3610_activate_speed_profile(false);
        default:
            LOG_ERR("Unknown profile type: %d", cfg->profile_type);
            return -EINVAL;
    }
}

static const struct behavior_driver_api behavior_pmw3610_profile_driver_api = {
    .binding_pressed = on_pmw3610_profile_binding_pressed,
    .binding_released = on_pmw3610_profile_binding_released,
};

static int behavior_pmw3610_profile_init(const struct device *dev) {
    LOG_DBG("PMW3610 profile behavior init");
    return 0;
}

#define PMW3610_PROFILE_INST(n)                                                            \
    static const struct behavior_pmw3610_profile_config behavior_pmw3610_profile_config_##n = { \
        .profile_type = DT_ENUM_IDX(DT_DRV_INST(n), profile_type),                          \
    };                                                                                     \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_pmw3610_profile_init, NULL, NULL,                  \
                          &behavior_pmw3610_profile_config_##n,                          \
                          POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,              \
                          &behavior_pmw3610_profile_driver_api);

DT_INST_FOREACH_STATUS_OKAY(PMW3610_PROFILE_INST)

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */ 