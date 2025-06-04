#include "esphome/core/defines.h"
#ifdef USE_LOCK
#include "lock.h"

namespace esphome {
namespace homekit {

#ifdef USE_HOMEKEY
readerData_t LockEntity::readerData;
nvs_handle LockEntity::savedHKdata;
pn532::PN532* LockEntity::nfc_ctx;

void LockEntity::set_hk_hw_finish(HKFinish color) {
    ESP_LOGI("homekit", "SELECTED HK FINISH: %d", static_cast<int>(color));
    hkFinishTlvData = std::make_unique<hap_tlv8_val_t>();
    hkFinishTlvData->val_len = 1;  // Using val_len
    hkFinishTlvData->val = new uint8_t[1];  // Using val
    hkFinishTlvData->val[0] = static_cast<uint8_t>(color);
}
#endif

void LockEntity::on_lock_update(lock::Lock* obj) {
    ESP_LOGD("on_lock_update", "%s state: %s", obj->get_name().c_str(), lock_state_to_string(obj->state));
    hap_acc_t* acc = hap_acc_get_by_aid(hap_get_unique_aid(std::to_string(obj->get_object_id_hash()).c_str()));
    hap_serv_t* hs = hap_acc_get_serv_by_uuid(acc, HAP_SERV_UUID_LOCK_MECHANISM);
    hap_char_t* current_state = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_LOCK_CURRENT_STATE);
    hap_char_t* target_state = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_LOCK_TARGET_STATE);
    hap_val_t c;
    hap_val_t t;
    
    if (obj->state == lock::LockState::LOCK_STATE_LOCKED || obj->state == lock::LockState::LOCK_STATE_UNLOCKED) {
        c.i = obj->state % 2;
        t.i = obj->state % 2;
        hap_char_update_val(current_state, &c);
        hap_char_update_val(target_state, &t);
    }
    else if (obj->state == lock::LockState::LOCK_STATE_LOCKING || obj->state == lock::LockState::LOCK_STATE_UNLOCKING) {
        t.i = (obj->state % 5) % 3;
        hap_char_update_val(target_state, &t);
    }
    else if (obj->state == lock::LockState::LOCK_STATE_JAMMED) {
        c.i = obj->state;
        hap_char_update_val(current_state, &c);
    }
}

int LockEntity::lock_write(hap_write_data_t write_data[], int count, void* serv_priv, void* write_priv) {
    lock::Lock* lockPtr = (lock::Lock*)serv_priv;
    ESP_LOGD("lock_write", "Write called for Accessory '%s'(%s)", lockPtr->get_name().c_str(), 
             std::to_string(lockPtr->get_object_id_hash()).c_str());
    
    int i, ret = HAP_SUCCESS;
    hap_write_data_t* write;
    
    for (i = 0; i < count; i++) {
        write = &write_data[i];
        if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_LOCK_TARGET_STATE)) {
            ESP_LOGD("lock_write", "Target State req: %d", write->val.i);
            hap_char_update_val(write->hc, &(write->val));
            hap_char_t* c = hap_serv_get_char_by_uuid(hap_char_get_parent(write->hc), 
                           HAP_CHAR_UUID_LOCK_CURRENT_STATE);
            ESP_LOGD("lock_write", "Current State: %d", hap_char_get_val(c)->i);
            hap_char_update_val(c, &(write->val));
            write->val.i ? lockPtr->lock() : lockPtr->unlock();
            *(write->status) = HAP_STATUS_SUCCESS;
        } else {
            *(write->status) = HAP_STATUS_RES_ABSENT;
        }
    }
    return ret;
}

int LockEntity::acc_identify(hap_acc_t* ha) {
    ESP_LOGI("homekit", "Accessory identified");
    return HAP_SUCCESS;
}

LockEntity::LockEntity(lock::Lock* lockPtr) : ptrToLock(lockPtr) {}

void LockEntity::setInfo(std::map<AInfo, const char*> info) {
    std::map<AInfo, const char*> merged_info;
    merged_info.merge(info);
    merged_info.merge(this->accessory_info);
    this->accessory_info.swap(merged_info);
}

void LockEntity::setup() {
    hap_acc_cfg_t acc_cfg = {
        .name = strdup(accessory_info[AInfo::NAME] ? accessory_info[AInfo::NAME] : ptrToLock->get_name().c_str()),
        .model = strdup(accessory_info[AInfo::MODEL]),
        .manufacturer = strdup(accessory_info[AInfo::MANUFACTURER]),
        .serial_num = strdup(accessory_info[AInfo::SN] ? accessory_info[AInfo::SN] : 
                            std::to_string(ptrToLock->get_object_id_hash()).c_str()),
        .fw_rev = strdup(accessory_info[AInfo::FW_REV]),
        .hw_rev = NULL,
#ifdef USE_HOMEKEY
        .hw_finish = hkFinishTlvData.get(),
#else
        .hw_finish = NULL,
#endif
        .pv = strdup("1.1.0"),
        .cid = HAP_CID_BRIDGE,
        .identify_routine = acc_identify,
    };

    hap_acc_t* accessory = hap_acc_create(&acc_cfg);
    hap_serv_t* lockMechanism = hap_serv_lock_mechanism_create(ptrToLock->state, ptrToLock->state);

    ESP_LOGD("homekit", "ID HASH: %lu", ptrToLock->get_object_id_hash());
    hap_serv_set_priv(lockMechanism, ptrToLock);
    hap_serv_set_write_cb(lockMechanism, lock_write);
    hap_acc_add_serv(accessory, lockMechanism);

    hap_add_bridged_accessory(accessory, 
                             hap_get_unique_aid(std::to_string(ptrToLock->get_object_id_hash()).c_str()));
    
    if (!ptrToLock->is_internal())
        ptrToLock->add_on_state_callback([this]() { this->on_lock_update(ptrToLock); });

    ESP_LOGI("homekit", "Lock '%s' linked to HomeKit", ptrToLock->get_name().c_str());
}

} // namespace homekit
} // namespace esphome
#endif
