/* temperture.h
 * Description: Header for STM32WB55 device read functions
 *
 * Author: Brian Schwantes
 * Contact: bschwant@iu.edu
 *
 */

void temp_command(char *);
uint32_t read_temp(void);