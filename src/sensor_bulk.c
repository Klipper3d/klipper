// Helper code for collecting and sending bulk sensor measurements
//
// Copyright (C) 2020-2023  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "command.h" // sendf
#include "sensor_bulk.h" // sensor_bulk_report

// Reset counters
void
sensor_bulk_reset(struct sensor_bulk *sb)
{
    sb->sequence = 0;
    sb->possible_overflows = 0;
    sb->data_count = 0;
}

// Report local measurement buffer
void
sensor_bulk_report(struct sensor_bulk *sb, uint8_t oid)
{
    sendf("sensor_bulk_data oid=%c sequence=%hu data=%*s"
          , oid, sb->sequence, sb->data_count, sb->data);
    sb->data_count = 0;
    sb->sequence++;
}

// Report buffer and fifo status
void
sensor_bulk_status(struct sensor_bulk *sb, uint8_t oid
                   , uint32_t time1, uint32_t query_ticks, uint32_t fifo)
{
    sendf("sensor_bulk_status oid=%c clock=%u query_ticks=%u next_sequence=%hu"
          " buffered=%u possible_overflows=%hu"
          , oid, time1, query_ticks, sb->sequence
          , sb->data_count + fifo, sb->possible_overflows);
}
