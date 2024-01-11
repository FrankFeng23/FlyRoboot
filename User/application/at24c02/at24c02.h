#ifndef __AT24C02_H_
#define __AT24C02_H_

#include "../../bsp/iic/iic.h"

#define AT24C02_ADR                         0xA0
#define AT24C02_PAGE_SIZE                   8


void iic_ee_write_bytes(uint8_t slave_adr,
                        uint8_t reg_adr,
                        uint8_t* pbuffer,
                        uint8_t len);
void iic_ee_read_bytes(uint8_t slave_adr,
                       uint8_t reg_adr,
                       uint8_t* pbuffer,
                       uint8_t len);





#endif

