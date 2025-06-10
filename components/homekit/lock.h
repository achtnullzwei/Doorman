#pragma once
#include <esphome/core/defines.h>

#ifdef USE_LOCK
#include <esphome/core/application.h>
#include <hap.h>
#include <map>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
#include "esphome/components/homekit_bridge/const.h"

#include "automation.h"

namespace esphome {
namespace homekit {

class LockEntity {
    static std::unordered_map<hap_acc_t*, LockEntity*> acc_instance_map;

private:
    lock::Lock* ptrToLock;
    std::vector<HKIdentifyTrigger *> triggers_identify_;

public:
    LockEntity(lock::Lock* lockPtr);
    void setup();
    void set_meta(std::map<AInfo, const char*> info);

    std::map<AInfo, const char*> accessory_info = {
        {NAME, NULL},
        {MODEL, "Lock"},
        {SN, NULL},
        {MANUFACTURER, "ESPHome"},
        {FW_REV, "0.1"}
    };

    void register_on_identify_trigger(HKIdentifyTrigger* trig);

protected:
    static int acc_identify(hap_acc_t* ha);
    static int lock_write(hap_write_data_t write_data[], int count, void* serv_priv, void* write_priv);
    void on_lock_update(lock::Lock* obj);
};

inline std::unordered_map<hap_acc_t*, LockEntity*> LockEntity::acc_instance_map;

} // namespace homekit
} // namespace esphome

#endif  // USE_LOCK
