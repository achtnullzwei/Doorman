#include "util.h"
#include "esphome.h"
#include "esphome/core/helpers.h"
#include <algorithm>
#include <cctype>

namespace esphome
{
    namespace tc_bus
    {
        const SettingMapping setting_mappings[] = {
            {SETTING_RINGTONE_FLOOR_CALL, "RINGTONE_FLOOR_CALL"},
            {SETTING_RINGTONE_ENTRANCE_DOOR_CALL, "RINGTONE_ENTRANCE_DOOR_CALL"},
            {SETTING_RINGTONE_SECOND_ENTRANCE_DOOR_CALL, "RINGTONE_SECOND_ENTRANCE_DOOR_CALL"},
            {SETTING_RINGTONE_INTERNAL_CALL, "RINGTONE_INTERNAL_CALL"},
            {SETTING_VOLUME_RINGTONE, "VOLUME_RINGTONE"},
            {SETTING_VOLUME_HANDSET_DOOR_CALL, "VOLUME_HANDSET_DOOR_CALL"},
            {SETTING_VOLUME_HANDSET_INTERNAL_CALL, "VOLUME_HANDSET_INTERNAL_CALL"}
        };

        SettingType string_to_setting_type(const char* str)
        {
            if (!str) return SETTING_UNKNOWN;

            for (const auto& mapping : setting_mappings) {
                const char* a = str;
                const char* b = mapping.name;
                while (*a && *b) {
                    if (toupper(*a) != toupper(*b)) break;
                    ++a; ++b;
                }
                if (*a == '\0' && *b == '\0') {
                    return mapping.type;
                }
            }
            return SETTING_UNKNOWN;
        }

        const char* setting_type_to_string(SettingType type)
        {
            for (const auto& mapping : setting_mappings) {
                if (mapping.type == type) return mapping.name;
            }
            return "UNKNOWN";
        }

        Model identifier_string_to_model(const uint8_t& device_group, const char* model_key, const uint8_t& hw_version, const uint32_t& fw_version)
        {
            if(device_group == 0 || device_group == 1)
            {
                if (strcmp(model_key, "000") == 0) return MODEL_IS_ISH3030;
                else if (strcmp(model_key, "010") == 0) return MODEL_IS_ISW3030;
                else if (strcmp(model_key, "001") == 0) return MODEL_IS_ISH3230;
                else if (strcmp(model_key, "011") == 0) return MODEL_IS_ISW3230;
                else if (strcmp(model_key, "003") == 0) return MODEL_IS_ISH3130;
                else if (strcmp(model_key, "013") == 0) return MODEL_IS_ISW3130;
                else if (strcmp(model_key, "015") == 0) return MODEL_IS_ISW3330;
                else if (strcmp(model_key, "002") == 0) return MODEL_IS_ISH3022;
                else if (strcmp(model_key, "017") == 0) return MODEL_IS_ISW3340;
                else if (strcmp(model_key, "800") == 0) return MODEL_IS_IVH3222;
                else if (strcmp(model_key, "900") == 0) return MODEL_IS_IVH4222;
                else if (strcmp(model_key, "B00") == 0) return MODEL_IS_IMM1000;
                else if (strcmp(model_key, "200") == 0) return MODEL_IS_ISW4100;
                else if (strcmp(model_key, "201") == 0) return MODEL_IS_IMM2100;
                else if (strcmp(model_key, "020") == 0 || strcmp(model_key, "021") == 0 || strcmp(model_key, "022") == 0 || strcmp(model_key, "023") == 0 || strcmp(model_key, "024") == 0 || strcmp(model_key, "025") == 0 || strcmp(model_key, "026") == 0 || strcmp(model_key, "027") == 0)
                    return MODEL_IS_ISW5010;

                else if (strcmp(model_key, "030") == 0 || strcmp(model_key, "031") == 0 || strcmp(model_key, "032") == 0)
                    return MODEL_IS_IVW511X;

                else if (strcmp(model_key, "03A") == 0 || strcmp(model_key, "03B") == 0 || strcmp(model_key, "03C") == 0 || strcmp(model_key, "03D") == 0 || strcmp(model_key, "03F") == 0)
                    return MODEL_IS_IVW521X;

                else if (strcmp(model_key, "028") == 0 || strcmp(model_key, "02B") == 0 || strcmp(model_key, "02F") == 0)
                    return MODEL_IS_ISW5020;

                else if (strcmp(model_key, "068") == 0 || strcmp(model_key, "06F") == 0)
                    return MODEL_IS_ISW5030;

                else if (strcmp(model_key, "060") == 0) return MODEL_IS_ISW5033;

                else if (strcmp(model_key, "070") == 0 || strcmp(model_key, "071") == 0 || strcmp(model_key, "072") == 0 || strcmp(model_key, "073") == 0 || strcmp(model_key, "074") == 0 || strcmp(model_key, "075") == 0 || strcmp(model_key, "076") == 0 || strcmp(model_key, "077") == 0)
                    return MODEL_IS_ISW6031;

                else if (strcmp(model_key, "080") == 0 || strcmp(model_key, "081") == 0 || strcmp(model_key, "082") == 0 || strcmp(model_key, "083") == 0 || strcmp(model_key, "084") == 0 || strcmp(model_key, "085") == 0 || strcmp(model_key, "086") == 0 || strcmp(model_key, "087") == 0)
                    return MODEL_IS_ISW7030;

                else if (strcmp(model_key, "088") == 0 || strcmp(model_key, "089") == 0 || strcmp(model_key, "08A") == 0 || strcmp(model_key, "08B") == 0 || strcmp(model_key, "08C") == 0 || strcmp(model_key, "08D") == 0 || strcmp(model_key, "08E") == 0 || strcmp(model_key, "08F") == 0)
                    return MODEL_IS_IVW7510;

                else if (strcmp(model_key, "180") == 0 || strcmp(model_key, "181") == 0 || strcmp(model_key, "182") == 0 || strcmp(model_key, "183") == 0 || strcmp(model_key, "184") == 0 || strcmp(model_key, "185") == 0 || strcmp(model_key, "186") == 0 || strcmp(model_key, "187") == 0)
                    return MODEL_IS_ISH7030;

                else if (strcmp(model_key, "188") == 0 || strcmp(model_key, "189") == 0 || strcmp(model_key, "18A") == 0 || strcmp(model_key, "18B") == 0 || strcmp(model_key, "18C") == 0 || strcmp(model_key, "18D") == 0 || strcmp(model_key, "18E") == 0 || strcmp(model_key, "18F") == 0)
                    return MODEL_IS_IVH7510;

                else if (strcmp(model_key, "078") == 0 || strcmp(model_key, "079") == 0 || strcmp(model_key, "07A") == 0 || strcmp(model_key, "07B") == 0 || strcmp(model_key, "07C") == 0 || strcmp(model_key, "07D") == 0 || strcmp(model_key, "07E") == 0 || strcmp(model_key, "07F") == 0)
                    return MODEL_IS_ISW6010;

                else if (strcmp(model_key, "058") == 0 || strcmp(model_key, "059") == 0 || strcmp(model_key, "05A") == 0 || strcmp(model_key, "05B") == 0 || strcmp(model_key, "05C") == 0 || strcmp(model_key, "05D") == 0 || strcmp(model_key, "05E") == 0 || strcmp(model_key, "05F") == 0)
                    return MODEL_IS_IVW6511;

                else if (strcmp(model_key, "C70") == 0 || strcmp(model_key, "C71") == 0 || strcmp(model_key, "C72") == 0 || strcmp(model_key, "C73") == 0 || strcmp(model_key, "C74") == 0 || strcmp(model_key, "C75") == 0 || strcmp(model_key, "C76") == 0 || strcmp(model_key, "C77") == 0)
                    return MODEL_IS_ISW7030;

                else if (strcmp(model_key, "C90") == 0 || strcmp(model_key, "C91") == 0 || strcmp(model_key, "C92") == 0 || strcmp(model_key, "C93") == 0 || strcmp(model_key, "C94") == 0 || strcmp(model_key, "C95") == 0 || strcmp(model_key, "C96") == 0 || strcmp(model_key, "C97") == 0)
                    return MODEL_IS_ISWM7000;

                else if (strcmp(model_key, "C80") == 0 || strcmp(model_key, "C81") == 0 || strcmp(model_key, "C82") == 0 || strcmp(model_key, "C83") == 0 || strcmp(model_key, "C84") == 0 || strcmp(model_key, "C85") == 0 || strcmp(model_key, "C86") == 0 || strcmp(model_key, "C87") == 0)
                    return MODEL_IS_IVWM7000;

                else if (strcmp(model_key, "800") == 0 || strcmp(model_key, "805") == 0) return MODEL_IS_IVW2210;
                else if (strcmp(model_key, "807") == 0) return MODEL_IS_IVW2211;
                else if (strcmp(model_key, "80C") == 0) return MODEL_IS_IVW2212;
                else if (strcmp(model_key, "810") == 0) return MODEL_IS_IVW2220;
                else if (strcmp(model_key, "815") == 0) return MODEL_IS_IVW2221;
                else if (strcmp(model_key, "820") == 0) return MODEL_IS_IVW3011;
                else if (strcmp(model_key, "830") == 0) return MODEL_IS_IVW3012;

                else if (strcmp(model_key, "C01") == 0) return MODEL_IS_VMH;
                else if (strcmp(model_key, "C00") == 0) return MODEL_IS_VML;
                else if (strcmp(model_key, "C02") == 0) return MODEL_IS_VMF;
                else if (strcmp(model_key, "400") == 0) return MODEL_IS_ISW42X0;
                else if (strcmp(model_key, "410") == 0) return MODEL_IS_TKIS;
                else if (strcmp(model_key, "420") == 0) return MODEL_IS_TKISV;
                else if (strcmp(model_key, "208") == 0) return MODEL_IS_CAIXXXX;
                else if (strcmp(model_key, "809") == 0) return MODEL_IS_CAI2000;
                else if (strcmp(model_key, "280") == 0) {
                    return (fw_version >= 512) ? MODEL_IS_VTC42V2 : MODEL_IS_VTC40;
                }
                else if (strcmp(model_key, "281") == 0) {
                    return (fw_version >= 512) ? MODEL_IS_TC40V2 : MODEL_IS_TC40;
                }
            
                else if (strcmp(model_key, "194") == 0) return MODEL_IS_IVW9030;
                else if (strcmp(model_key, "1E8") == 0) return MODEL_IS_IVW9010;
                else if (strcmp(model_key, "1EA") == 0) return MODEL_IS_IVW9110;
                else if (strcmp(model_key, "1E9") == 0) return MODEL_IS_IVW9011;
                else if (strcmp(model_key, "1B3") == 0 || strcmp(model_key, "1B4") == 0 || strcmp(model_key, "1B5") == 0)
                    return MODEL_IS_IVE70;
            }
            else if(device_group == 4)
            {
                if (strcmp(model_key, "008") == 0) return MODEL_CTRL_BVS30;
                else if (strcmp(model_key, "010") == 0) return MODEL_CTRL_NBV3210;
                else if (strcmp(model_key, "009") == 0) return MODEL_CTRL_VBVS30;
                else if (strcmp(model_key, "D2D") == 0) return MODEL_CTRL_NBV2600;
            }
            else
            {
                // Other device groups
                // Not implemented
            }

            return MODEL_NONE;
        }

        const ModelMapping model_mappings[] = {
            {MODEL_IS_ISW3030, "TCS ISW3030 / Koch TC50 / Scantron Stilux"},
            {MODEL_IS_ISW3130, "TCS ISW3130 / Koch TC50P"},
            {MODEL_IS_ISW3230, "TCS ISW3230 / Koch TC50 GFA"},
            {MODEL_IS_ISW3330, "TCS ISW3330 / Koch TC50 BW"},
            {MODEL_IS_ISW3340, "TCS ISW3340"},
            {MODEL_IS_ISW5010, "TCS ISW5010 / Koch TC60"},
            {MODEL_IS_ISW5020, "TCS ISW5020"},
            {MODEL_IS_ISW5030, "TCS ISW5030"},
            {MODEL_IS_ISW5031, "TCS ISW5031"},
            {MODEL_IS_ISW5033, "TCS ISW5033"},
            {MODEL_IS_IVW511X, "TCS IVW511x / Koch VTC60 / Scantron VIVO"},
            {MODEL_IS_IVW521X, "TCS IVW521x / Koch VTC60/2D"},
            {MODEL_IS_ISW6031, "TCS ISW6031"},
            {MODEL_IS_ISW7030, "TCS ISW7030 / Koch TC70"},
            {MODEL_IS_IVW7510, "TCS IVW7510 / Koch VTC70"},
            {MODEL_IS_ISH7030, "TCS ISH7030 / Koch TCH70"},
            {MODEL_IS_IVH7510, "TCS IVH7510 / Koch VTCH70"},
            {MODEL_IS_ISW6010, "TCS ISW6010"},
            {MODEL_IS_IVW6511, "TCS IVW6511"},
            {MODEL_IS_ISWM7000, "TCS ISWM7000"},
            {MODEL_IS_IVWM7000, "TCS IVWM7000"},
            {MODEL_IS_ISW4100, "TCS ISW4100 / Koch TC31"},
            {MODEL_IS_IMM2100, "TCS IMM2100 / Koch TCE31"},
            {MODEL_IS_IVW2210, "TCS IVW2210 / Koch Ecoos"},
            {MODEL_IS_IVW2211, "TCS IVW2211 / Koch Ecoos"},
            {MODEL_IS_IVW2212, "TCS IVW2212 / Koch Ecoos / Scantron SLIM60T"},
            {MODEL_IS_VTC42V2, "TCS VTC42V2"},
            {MODEL_IS_TC40V2, "TCS TC40V2"},
            {MODEL_IS_VTC40, "TCS VTC40"},
            {MODEL_IS_TC40, "TCS TC40"},
            {MODEL_IS_TC2000, "TCS TC2000"},
            {MODEL_IS_TC20P, "TCS TC20P"},
            {MODEL_IS_TC20F, "TCS TC20F"},
            {MODEL_IS_ISH3022, "TCS ISH3022"},
            {MODEL_IS_ISH3130, "TCS ISH3130 / Koch TCH50P / Scantron LuxPlus"},
            {MODEL_IS_ISH3230, "TCS ISH3230 / Koch TCH50 GFA"},
            {MODEL_IS_ISH3030, "TCS ISH3030 / Koch TCH50 / Scantron Lux2"},
            {MODEL_IS_ISH1030, "TCS ISH1030 / Koch TTS25"},
            {MODEL_IS_TTCXX, "TCS TTC-XX"},
            {MODEL_IS_TTSXX, "TCS TTS-XX"},
            {MODEL_IS_IMM1000, "TCS IMM1000 / Koch TCH30"},
            {MODEL_IS_IMM1100, "TCS IMM1100 / Koch TCHE30"},
            {MODEL_IS_IMM1300, "TCS IMM1300 / Koch VTCH30"},
            {MODEL_IS_IMM1500, "TCS IMM1500"},
            {MODEL_IS_IMM1310, "TCS IMM1310 / Koch VTCHE30"},
            {MODEL_IS_IMM1110, "TCS IMM1110 / Koch TCHEE30"},
            {MODEL_IS_IVH3222, "TCS IVH3222 / Koch VTCH50 / Scantron VLux"},
            {MODEL_IS_IVH4222, "TCS IVH4222 / Koch VTCH50/2D"},
            {MODEL_IS_IVW2220, "TCS IVW2220 / Koch Sky"},
            {MODEL_IS_IVW2221, "TCS IVW2221 / Koch Sky R1.00"},
            {MODEL_IS_IVW3011, "TCS IVW3011 / Koch Skyline Plus"},
            {MODEL_IS_IVW3012, "TCS IVW3012 / Koch Skyline/Aldup"},
            {MODEL_IS_VMH, "TCS VMH / Koch VMH"},
            {MODEL_IS_VML, "TCS VML / Koch VML"},
            {MODEL_IS_VMF, "TCS VMF / Koch VMF"},
            {MODEL_IS_TKIS, "Jung TKIS"},
            {MODEL_IS_TKISV, "Jung TKISV"},
            {MODEL_IS_CAIXXXX, "TCS CAIXXXX / Koch CAIXXXX"},
            {MODEL_IS_CAI2000, "TCS CAI2000 / Koch Carus"},
            {MODEL_IS_ISW42X0, "TCS ISW42X0"},
            {MODEL_IS_IVW9010, "TCS IVW9010"},
            {MODEL_IS_IVW9011, "TCS IVW9011 / Koch VTP10"},
            {MODEL_IS_IVW9110, "TCS IVW9110"},
            {MODEL_IS_IVW9030, "TCS IVW9030 / Scantron SLIM50T"},
            {MODEL_IS_IVE70, "TCS IVE70"},
            {MODEL_IS_DEBUG_0, "DEBUG IS0"},
            {MODEL_IS_DEBUG_1, "DEBUG IS1"},
            {MODEL_AS_DEBUG, "DEBUG AS"},
            {MODEL_CTRL_BVS20, "TCS BVS20"},
            {MODEL_CTRL_BVS30, "TCS BVS30"},
            {MODEL_CTRL_NBV3210, "TCS NBV3210"},
            {MODEL_CTRL_VBVS30, "TCS VBVS30"},
            {MODEL_CTRL_NBV2600, "TCS NBV2600"},
            {MODEL_CTRL_DEBUG, "DEBUG CONTROLLER"},
            {MODEL_EXT_TRE2, "TCS TRE2"},
            {MODEL_EXT_DEBUG, "DEBUG EXTENSION"}
        };

        Model string_to_model(const char* str) {
            if (!str) return MODEL_NONE;

            for (const auto& mapping : model_mappings) {
                if (strcmp(str, mapping.name) == 0) {
                    return mapping.model;
                }
            }
            return MODEL_NONE;
        }

        const char* model_to_string(Model model) {
            for (const auto& mapping : model_mappings) {
                if (mapping.model == model) {
                    return mapping.name;
                }
            }
            return "None";
        }

        ModelData getModelData(Model model)
        {
            ModelData modelData{};
            modelData.model = model;
            modelData.capabilities = 0;

            switch (model)
            {
                // Group 1
                case MODEL_IS_ISW3030: /* TC50 */
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_ISW3130: /* TC50P */
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_ISW3230: /* TC50 GFA */
                    modelData.device_group = 1;
                    modelData.memory_size = 40;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_ISW3330: /* TC50 BW */
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_ISW3340:
                    modelData.device_group = 1;
                    modelData.memory_size = 128;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_ISW5010: /* TC60 */
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_INTERNAL_CALL;
                    break;
                case MODEL_IS_ISW5020:
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_INTERNAL_CALL;
                    break;
                case MODEL_IS_ISW5030:
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_INTERNAL_CALL;
                    break;
                case MODEL_IS_ISW5031:
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_INTERNAL_CALL;
                    break;
                case MODEL_IS_ISW5033:
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_INTERNAL_CALL;
                    break;
                case MODEL_IS_IVW511X: /* VTC60 */
                    modelData.device_group = 1;
                    modelData.memory_size = 48;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_INTERNAL_CALL;
                    break;
                case MODEL_IS_IVW521X: /* VTC60/2D */
                    modelData.device_group = 1;
                    modelData.memory_size = 48;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_INTERNAL_CALL;
                    break;
                case MODEL_IS_ISW6031:
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_ISW7030: /* TC70 */
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IVW7510: /* VTC70 */
                    modelData.device_group = 1;
                    modelData.memory_size = 48;
                    break;
                case MODEL_IS_ISH7030: /* TCH70 */
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IVH7510: /* VTCH70 */
                    modelData.device_group = 1;
                    modelData.memory_size = 48;
                    break;
                case MODEL_IS_ISW6010:
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IVW6511:
                    modelData.device_group = 1;
                    modelData.memory_size = 48;
                    break;
                case MODEL_IS_ISWM7000:
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IVWM7000:
                    modelData.device_group = 1;
                    modelData.memory_size = 48;
                    break;
                case MODEL_IS_ISW4100: /* TC31 */
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IMM2100: /* TCE31 */
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IVW2210: /* Ecoos */
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    // Supported? Not documented
                    //modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_IVW2211: /* Ecoos */
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    // Supported? Not documented
                    //modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_IVW2212: /* Ecoos */
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    // Supported? Not documented
                    //modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_VTC42V2:
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_TC40V2:
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_VTC40:
                    modelData.device_group = 1;
                    modelData.memory_size = 40;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_TC40:
                    modelData.device_group = 1;
                    modelData.memory_size = 40;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_TC2000:
                    modelData.device_group = 1;
                    modelData.memory_size = 16;
                    break;
                case MODEL_IS_TC20P:
                    modelData.device_group = 1;
                    modelData.memory_size = 16;
                    break;
                case MODEL_IS_TC20F:
                    modelData.device_group = 1;
                    modelData.memory_size = 16;
                    break;
                case MODEL_IS_IVW2220:
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    break;
                case MODEL_IS_IVW2221:
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    break;
                case MODEL_IS_IVW3011:
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    break;
                case MODEL_IS_IVW3012:
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    break;
                case MODEL_IS_TKIS:
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    break;
                case MODEL_IS_TKISV:
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    break;
                case MODEL_IS_CAI2000:
                    modelData.device_group = 1;
                    modelData.memory_size = 64;
                    break;
                case MODEL_IS_CAIXXXX:
                    modelData.device_group = 1;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_ISW42X0:
                    modelData.device_group = 1;
                    modelData.memory_size = 40;
                    break;

                // Group 0
                case MODEL_IS_ISH3022: /* TCH50P */
                    modelData.device_group = 0;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_ISH3130: /* TCH50P */
                    modelData.device_group = 0;
                    modelData.memory_size = 40;
                    break;
                case MODEL_IS_ISH3230: /* TCH50 GFA */
                    modelData.device_group = 0;
                    modelData.memory_size = 40;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_ISH3030: /* TCH50 */
                    modelData.device_group = 0;
                    modelData.memory_size = 32;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_ISH1030: /* TTS25 */
                    modelData.device_group = 0;
                    modelData.memory_size = 16;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    break;
                case MODEL_IS_TTCXX:
                    modelData.device_group = 0;
                    modelData.memory_size = 16;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    break;
                case MODEL_IS_TTSXX:
                    modelData.device_group = 0;
                    modelData.memory_size = 16;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    break;
                case MODEL_IS_IMM1000: /* TCH30 */
                    modelData.device_group = 0;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IMM1100: /* TCHE30 */
                    modelData.device_group = 0;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IMM1300: /* VTCH30 */
                    modelData.device_group = 0;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IMM1500:
                    modelData.device_group = 0;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IMM1310: /* VTCHE30 */
                    modelData.device_group = 0;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IMM1110: /* TCHEE30 */
                    modelData.device_group = 0;
                    modelData.memory_size = 32;
                    break;
                case MODEL_IS_IVH3222: /* VTCH50 */
                    modelData.device_group = 0;
                    modelData.memory_size = 32;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    break;
                case MODEL_IS_IVH4222: /* VTCH50/2D */
                    modelData.device_group = 0;
                    modelData.memory_size = 32;
                    modelData.capabilities |= CAP_RINGTONE_ENTRANCE_DOOR_CALL;
                    modelData.capabilities |= CAP_RINGTONE_INTERNAL_CALL;
                    modelData.capabilities |= CAP_RINGTONE_FLOOR_CALL;
                    modelData.capabilities |= CAP_VOLUME_RINGTONE;
                    modelData.capabilities |= CAP_VOLUME_HANDSET_DOOR_CALL;
                    break;
                case MODEL_IS_VMH:
                    modelData.device_group = 0;
                    modelData.memory_size = 24;
                    break;
                case MODEL_IS_VML:
                    modelData.device_group = 0;
                    modelData.memory_size = 24;
                    break;
                case MODEL_IS_VMF:
                    modelData.device_group = 0;
                    modelData.memory_size = 24;
                    break;

                case MODEL_IS_IVW9010:
                case MODEL_IS_IVW9011:
                case MODEL_IS_IVW9110:
                case MODEL_IS_IVW9030:
                case MODEL_IS_IVE70:
                    modelData.device_group = 0;
                    modelData.memory_size = 0;
                    break;

                case MODEL_IS_DEBUG_0:
                    modelData.device_group = 0;
                    modelData.memory_size = 128;
                    break;
                case MODEL_IS_DEBUG_1:
                    modelData.device_group = 1;
                    modelData.memory_size = 128;
                    break;

                // Group 2
                case MODEL_AS_DEBUG:
                    modelData.device_group = 2;
                    modelData.memory_size = 128;
                    break;

                // Group 3
                // Not implemented

                // Group 4
                case MODEL_CTRL_BVS20:
                case MODEL_CTRL_BVS30:
                case MODEL_CTRL_NBV3210:
                case MODEL_CTRL_VBVS30:
                case MODEL_CTRL_NBV2600:
                    modelData.device_group = 4;
                    modelData.memory_size = 0;
                    break;
                case MODEL_CTRL_DEBUG:
                    modelData.device_group = 4;
                    modelData.memory_size = 128;
                    break;

                // Group 5
                // Not implemented

                // Group 6
                case MODEL_EXT_TRE2:
                    modelData.device_group = 6;
                    modelData.memory_size = 0;
                    break;

                case MODEL_EXT_DEBUG:
                    modelData.device_group = 6;
                    modelData.memory_size = 128;
                    break;

                default:
                    break;
            }

            return modelData;
        }

        SettingCellData getSettingCellData(SettingType setting, Model model)
        {
            SettingCellData data{};
            ModelData model_data = getModelData(model);

            // Only set data if model supports the capability
            if (setting == SETTING_RINGTONE_ENTRANCE_DOOR_CALL && (model_data.capabilities & CAP_RINGTONE_ENTRANCE_DOOR_CALL))
            {
                data.index = 3;
                data.left_nibble = true;
            }
            else if (setting == SETTING_RINGTONE_INTERNAL_CALL && (model_data.capabilities & CAP_RINGTONE_INTERNAL_CALL))
            {
                data.index = 6;
                data.left_nibble = true;
            }
            else if (setting == SETTING_RINGTONE_FLOOR_CALL && (model_data.capabilities & CAP_RINGTONE_FLOOR_CALL))
            {
                data.index = 9;
                data.left_nibble = true;
            }
            else if (setting == SETTING_RINGTONE_SECOND_ENTRANCE_DOOR_CALL && (model_data.capabilities & CAP_RINGTONE_SECOND_ENTRANCE_DOOR_CALL))
            {
                data.index = 12;
                data.left_nibble = true;
            }
            else if (setting == SETTING_VOLUME_RINGTONE && (model_data.capabilities & CAP_VOLUME_RINGTONE))
            {
                data.index = 20;
                data.left_nibble = false;
            }
            else if (setting == SETTING_VOLUME_HANDSET_DOOR_CALL && (model_data.capabilities & CAP_VOLUME_HANDSET_DOOR_CALL))
            {
                data.index = 21;
                data.left_nibble = false;
            }
            else if (setting == SETTING_VOLUME_HANDSET_INTERNAL_CALL && (model_data.capabilities & CAP_VOLUME_HANDSET_INTERNAL_CALL))
            {
                data.index = 21;
                data.left_nibble = true;
            }

            return data;
        }

        const char* const ringtones[] = {
            "Ringtone 1",
            "Ringtone 2",
            "Ringtone 3",
            "Ringtone 4",
            "Ringtone 5",
            "Ringtone 6",
            "Ringtone 7",
            "Ringtone 8",
            "Ringtone 9",
            "Ringtone 10",
            "Ringtone 11",
            "Ringtone 12",
            "Ringtone 13"
        };
        
        uint8_t ringtone_to_int(const char* str)
        {
            for (uint8_t i = 0; i < sizeof(ringtones) / sizeof(ringtones[0]); ++i) {
                if (strcmp(str, ringtones[i]) == 0) {
                    return i;
                }
            }
            return 0;
        }

        const char* int_to_ringtone(uint8_t ringtone)
        {
            if (ringtone > 12) ringtone = 0;
            return ringtones[ringtone];
        }

    }  // namespace tc_bus
}  // namespace esphome