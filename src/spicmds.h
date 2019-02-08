#ifndef __SPICMDS_H
#define __SPICMDS_H

struct spi_command_device {
    struct spidev_s *hardware_spi;
    struct software_spi_config *software_spi;
    uint8_t is_hardware;
};

struct spi_command_device *spidev_oid_lookup(uint8_t oid);


#endif // spicmds.h


