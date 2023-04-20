#ifndef CONFIG_H
#define CONFIG_H
#include <WString.h>
#ifdef __cplusplus
struct MPU_Config
{
    int16_t x_gyro_offset;
    int16_t y_gyro_offset;
    int16_t z_gyro_offset;

    int16_t x_accel_offset;
    int16_t y_accel_offset;
    int16_t z_accel_offset;
};

struct Config
{
  /*   String ssid;
    String password;
    String cityname;              // 显示的城市
    String language;              // 天气查询的地址编码
    String weather_key;           // 知心天气api_key（秘钥）
    String tianqi_appid;          // tianqiapid 的 appid
    String tianqi_appsecret;      // tianqiapid 的 appsecret
    String tianqi_addr;           // tianqiapid 的地址（填中文）
    String bili_uid;              // bilibili的uid */
    uint8_t backLight;            // background color（1-100）
    uint8_t rotation;             // screen rotation direction
    uint8_t auto_calibration_mpu; // mpu auto correction 0: no 1: yes
    MPU_Config mpu_config;        // mpu configuration
};

void config_read(const char *file_path, Config *cfg);

void config_save(const char *file_path, Config *cfg);

void mpu_config_read(const char *file_path, Config *cfg);

void mpu_config_save(const char *file_path, Config *cfg);

#endif
#endif