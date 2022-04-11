/* battery.h
 * Description: Header file for device battery read functions
 *
 * Author: Bryce Himebaugh and Brian Schwantes
 * Contact: bhimebau@indiana.edu | bschwant@iu.edu
 *
 */

void batt_command(char *);
uint32_t read_vrefint(void);