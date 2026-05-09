#ifndef __SENSOR_BULK_H
#define __SENSOR_BULK_H

struct sensor_bulk {
    uint16_t sequence, possible_overflows;
    uint8_t data_count;
    uint8_t data[51];
};

void sensor_bulk_reset(struct sensor_bulk *sb);
void sensor_bulk_report(struct sensor_bulk *sb, uint8_t oid);
void sensor_bulk_status(struct sensor_bulk *sb, uint8_t oid
                        , uint32_t time1, uint32_t query_ticks, uint32_t fifo);

#endif // sensor_bulk.h
