---
description: Reference guide for all Nuki Smart Lock entities, detailing their functions, usage, and configuration.
---

# Nuki Lock Entities
On this page, you can view all the entities related to the Nuki Lock.

## Sensors

### Battery Level <Badge type="tip" text="nuki_battery_level" />
Reports the battery level of the paired Nuki Smart Lock.

### Bluetooth Signal <Badge type="tip" text="nuki_bluetooth_signal_strength" />
Reports the bluetooth strength of the paired Nuki Smart Lock.


## Binary Sensors

### Battery Critical <Badge type="tip" text="nuki_battery_critical" />
Activates when the paired Nuki Smart Lock has a critical battery level.

### Door Sensor <Badge type="tip" text="nuki_door_sensor" /> <Badge type="info" text="Disabled by default" />
No description yet.


## Text Sensors

### Door Sensor State <Badge type="tip" text="nuki_door_sensor_state" /> <Badge type="info" text="Disabled by default" />
Reports the door sensor state of the paired Nuki Smart Lock.

### Last Unlock User <Badge type="tip" text="nuki_last_unlock_user" />
Reports the last user who locked or unlocked the Nuki Smart Lock.

### Last Lock Action <Badge type="tip" text="nuki_last_lock_action" /> <Badge type="info" text="Disabled by default" />
Reports the last lock action of the Nuki Smart Lock.

### Last Lock Action Trigger <Badge type="tip" text="nuki_last_lock_action_trigger" /> <Badge type="info" text="Disabled by default" />
Reports the last log action trigger of the Nuki Smart Lock.


## Switches

### Auto unlatch <Badge type="tip" text="nuki_auto_unlatch" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Auto unlatch setting.

### Button: Bluetooth Pairing <Badge type="tip" text="nuki_button_pairing_enabled" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Button locking operations setting.

### Button: Locking operations <Badge type="tip" text="nuki_button_locking_operations" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Button locking operations setting.

### LED: Signal <Badge type="tip" text="nuki_led_enabled" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock LED Signal setting.

### Night Mode <Badge type="tip" text="nuki_night_mode" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Night Mode setting.

### Night Mode: Auto Lock <Badge type="tip" text="nuki_night_mode_auto_lock_enabled" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Night Mode Auto Lock setting.

### Night Mode: Reject Auto Unlock <Badge type="tip" text="nuki_night_mode_auto_unlock_disabled" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Night Mode Reject Auto Unlock setting.

### Night Mode: Lock at Start Time <Badge type="tip" text="nuki_night_mode_immediate_lock_on_start" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Night Mode Lock at Start Time setting.

### Auto Lock <Badge type="tip" text="nuki_auto_lock" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Auto Lock setting.

### Auto Lock: Immediately <Badge type="tip" text="nuki_immediate_auto_lock" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Auto Lock Immediately setting.

### Auto Unlock: Disable <Badge type="tip" text="nuki_auto_unlock_disabled" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Auto Unlock Disable setting.

### Single Lock <Badge type="tip" text="nuki_single_lock" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Single Lock setting.

### Detached Cylinder <Badge type="tip" text="nuki_detached_cylinder" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock detached cylinder setting.


## Buttons

### Request Calibration <Badge type="tip" text="nuki_request_calibration" /> <Badge type="info" text="Disabled by default" />
Requests the calibration of the Nuki Smart Lock.


## Numbers

### LED: Brightness <Badge type="tip" text="nuki_led_brightness" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock LED Brightness setting.

### Unlatch Duration <Badge type="tip" text="nuki_unlatch_duration" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Unlatch Duration setting.

### Unlocked Position Offset <Badge type="tip" text="nuki_unlocked_position_offset" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Unlocked Position Offset setting.

### Single Locked Position Offset <Badge type="tip" text="nuki_single_locked_position_offset" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Single Locked Position Offset setting.

### Locked Position Offset <Badge type="tip" text="nuki_locked_position_offset" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Locked Position Offset setting.

### Unlocked To Locked Transition Offset <Badge type="tip" text="nuki_unlocked_to_locked_transition_offset" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Unlocked To Locked Transition Offset setting.


## Selects

### Button: Single Press Action <Badge type="tip" text="nuki_single_button_press_action" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Single Button Press Action setting.

##### Options:
- **No Action**
- **Intelligent**
- **Unlock**
- **Lock**
- **Unlatch**
- **Lock n Go**
- **Show Status**

### Button: Double Press Action <Badge type="tip" text="nuki_double_button_press_action" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Double Button Press Action setting.

##### Options:
- **No Action**
- **Intelligent**
- **Unlock**
- **Lock**
- **Unlatch**
- **Lock n Go**
- **Show Status**

### Fob: Action 1 <Badge type="tip" text="nuki_fob_action_1" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Fob Action 1 setting.

##### Options:
- **No Action**
- **Unlock**
- **Lock**
- **Lock n Go**
- **Intelligent**

### Fob: Action 2 <Badge type="tip" text="nuki_fob_action_2" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Fob Action 2 setting.

##### Options:
- **No Action**
- **Unlock**
- **Lock**
- **Lock n Go**
- **Intelligent**

### Fob: Action 3 <Badge type="tip" text="nuki_fob_action_3" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Fob Action 3 setting.

##### Options:
- **No Action**
- **Unlock**
- **Lock**
- **Lock n Go**
- **Intelligent**

### Advertising Mode <Badge type="tip" text="nuki_advertising_mode" /> <Badge type="info" text="Disabled by default" />
Controls the Nuki Smart Lock Advertising Mode setting.

##### Options:
- **Automatic**
- **Normal**
- **Slow**
- **Slowest**


## Locks

### Apartment Door <Badge type="tip" text="apartment_door" />
Represents the apartment door Nuki Smart Lock.