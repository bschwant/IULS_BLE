/* sample.h
 * Description: 
 *
 * Author: Bryce Himebaugh and Brian Schwantes
 *
 */

int sample(void);
int read_data_records(flash_status_t *);
int read_log_records(flash_status_t *);
int log(uint8_t *);
void print_log_command(char *);