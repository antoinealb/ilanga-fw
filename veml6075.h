#ifndef VEML6075_H
#define VEML6075_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef void (*veml7075_i2c_transmit_fn)(void *dev, uint8_t addr, const uint8_t *tx, size_t txbytes,
                                         uint8_t *rx, size_t rxbytes);

typedef struct {
    void *i2c_dev;
    veml7075_i2c_transmit_fn i2c_transmit;
} veml6075_dev_t;

enum veml6075_trigger_mode {
    VEML6075_TRIGGER_AUTO=0,
    VEML6075_TRIGGER_MANUAL=1
};

enum veml6075_integration_time {
    VEML6075_EXPOSURE_50MS=0,
    VEML6075_EXPOSURE_100MS=1,
    VEML6075_EXPOSURE_200MS=2,
    VEML6075_EXPOSURE_400MS=3,
    VEML6075_EXPOSURE_800MS=4
};

void veml6075_init(veml6075_dev_t *dev, veml7075_i2c_transmit_fn i2c_transmit, void *i2c_dev);

/** Configures the attached sensor.
 *
 * @param [in] dev The driver instance to configure.
 * @param [in] trigger The trigger mode to use (manual or automatic).
 * @param [in] it Integration time to use (50, 100, 200, 400 or 800 ms).
 * @param [in] use_high_dynamic Use increased dynamic mode.
 *
 * @note It is not quite clear what the increased dynamic mode is for.
 */
void veml6075_configure(veml6075_dev_t *dev,
                        enum veml6075_trigger_mode trigger,
                        enum veml6075_integration_time it,
                        bool use_high_dynamic);

/** Reads the value of the UVA register.
 *
 * @param [in] dev The driver instance to use.
 * @returns The raw value coming from the sensor.
 */
uint16_t veml6075_read_uva(veml6075_dev_t *dev);

/** Reads the value of the UVB register.
 *
 * @param [in] dev The driver instance to use.
 * @returns The raw value coming from the sensor.
 */
uint16_t veml6075_read_uvb(veml6075_dev_t *dev);


#ifdef __cplusplus
}
#endif

#endif
