#include <stm32mp1xx.h>

#include "command.h" // output
#include "board/pgm.h" // PROGMEM
#include "board/armcm_boot.h" // armcm_enable_irq

#include <string.h>
#include <stdalign.h>

#define DEFAULT_IRQ_PRIO    1U

#define IPCC_ALL_RX_BUF 	0x0000003FU
#define IPCC_ALL_TX_BUF 	0x003F0000U
#define IPCC_CHANNEL_1 		0x00000000U
#define IPCC_CHANNEL_2 		0x00000001U

#define VIRTIO_CONFIG_STATUS_ACK       		0x01
#define VIRTIO_CONFIG_STATUS_DRIVER    		0x02
#define VIRTIO_CONFIG_STATUS_DRIVER_OK 		0x04
#define VIRTIO_CONFIG_STATUS_NEEDS_RESET	0x40
#define VIRTIO_CONFIG_STATUS_FAILED    		0x80

#define VIRTIO_ID_RPMSG_            7
#define RPMSG_IPU_C0_FEATURES       1

#define NUM_RESOURCE_ENTRIES    	2
#define VRING_COUNT         		2
#define VRING_ALIGNMENT         	16
#define VRING_NUM_BUFFS      	   	16
#define VDEV_ID                		0xFF
#define VRING0_ID               	0
#define VRING1_ID               	1
#define VRING_USED_F_NO_NOTIFY  	1
#define VRING_AVAIL_F_NO_INTERRUPT  1

#define VRING_RX 					1
#define VRING_TX 					0

#define RPROC_MAX_NAME_LEN 		  	32
#define RPMSG_NAME_SIZE			  	32
#define RPMSG_TICK_COUNT          	15000000
#define RPMSG_TICKS_PER_INTERVAL  	1000

#define SHM_DEVICE_NAME         	"STM32_SHM"
#define RPMSG_SERVICE_NAME      	"rpmsg-tty"
#define RPMSG_NS_NAME      			"NS"
#define RPMSG_NS_EPT_ADDR			0x35
#define RPMSG_RESERVED_ADDRESSES	1024
#define RPMSG_ADDR_ANY				0xFFFFFFFF
#define RPMSG_BUF_HELD 				(1U << 31)

struct _vring_desc {
	volatile uint64_t addr;
	volatile uint32_t len;
	volatile uint16_t flags;
	volatile uint16_t next;
} __attribute__((__packed__));;

struct _vring_avail {
	volatile uint16_t flags;
	volatile uint16_t idx;
	volatile uint16_t ring[VRING_NUM_BUFFS];
} __attribute__((__packed__));

struct _vring_used_elem {
	union {
		volatile uint16_t event;
		volatile uint32_t id;
	};
	volatile uint32_t len;
}  __attribute__((__packed__));

struct _vring_used {
	volatile uint16_t flags;
	volatile uint16_t idx;
	volatile struct _vring_used_elem ring[VRING_NUM_BUFFS];
}  __attribute__((__packed__));

struct _vring {
	volatile struct _vring_desc desc[VRING_NUM_BUFFS];
	volatile struct _vring_avail avail;
	uint16_t reserved;
	alignas(VRING_ALIGNMENT) struct _vring_used used;
}__attribute__((__packed__));

struct _fw_rsc_vdev_vring {
	volatile struct _vring * da;
	uint32_t align;
	uint32_t num;
	uint32_t notifyid;
	uint32_t reserved;
} __attribute__((__packed__));


enum fw_resource_type {
	RSC_CARVEOUT = 0,
	RSC_DEVMEM = 1,
	RSC_TRACE = 2,
	RSC_VDEV = 3,
	RSC_LAST = 4,
	RSC_VENDOR_START = 128,
	RSC_VENDOR_END = 512,
};

struct _fw_rsc_vdev {
	uint32_t type;
	uint32_t id;
	uint32_t notifyid;
	uint32_t dfeatures;
	uint32_t gfeatures;
	uint32_t config_len;
	uint8_t status;
	uint8_t num_of_vrings;
	uint8_t reserved[2];
	struct _fw_rsc_vdev_vring vring[0];
} __attribute__((__packed__));

struct _fw_rsc_trace {
	uint32_t type;
	uint32_t da;
	uint32_t len;
	uint32_t reserved;
	uint8_t name[RPROC_MAX_NAME_LEN];
} __attribute__((__packed__));

struct _shared_resource_table {
	unsigned int version;
	unsigned int num;
	unsigned int reserved[2];
	unsigned int offset[NUM_RESOURCE_ENTRIES];

	volatile struct _fw_rsc_vdev vdev;
	volatile struct _fw_rsc_vdev_vring vrings[VRING_COUNT];
	struct _fw_rsc_trace cm_trace;
};

enum rpmsg_ns_flags {
	RPMSG_NS_CREATE = 0,
	RPMSG_NS_DESTROY = 1,
};

struct rpmsg_ns_msg {
	char name[RPMSG_NAME_SIZE];
	uint32_t addr;
	uint32_t flags;
} __attribute__((__packed__));

struct rpmsg_hdr {
	uint32_t src;
	uint32_t dst;
	uint32_t reserved;
	uint16_t len;
	uint16_t flags;
}__attribute__((__packed__));

struct rpmsg_buffer {
	struct rpmsg_hdr header;
	char data[];
} __attribute__((__packed__));

struct _rpmsg_endpoint;
typedef int (*_rpmsg_ept_cb)(uint8_t *data,size_t len, uint32_t src);

struct _rpmsg_endpoint {
	char name[RPMSG_NAME_SIZE];
	uint32_t addr;
	uint32_t dest_addr;
	_rpmsg_ept_cb cb;
};


// IRQ
void IPCC_RX1_IRQHandler(void);

// Private
void _virtqueue_kick(size_t vq_idx);
volatile struct rpmsg_buffer *_virtqueue_get_available_buffer(size_t vq_idx, uint16_t *avail_idx);
volatile struct rpmsg_buffer *_rpmsg_virtio_get_tx_payload_buffer(uint16_t*  idx, uint32_t *len);
void _virtqueue_add_consumed_buffer(size_t vq_idx, uint16_t head_idx);
int _rpmsg_virtio_send_offchannel_raw(uint32_t src, uint32_t dst, const void *data, int len);
static struct _rpmsg_endpoint *_rpmsg_get_endpoint_by_addr(uint32_t addr);
static void _rpmsg_virtio_rx_callback();
static struct _rpmsg_endpoint *_rpmsg_get_endpoint_by_name(const char *name, uint32_t dest_addr);
static int _rpmsg_virtio_ns_callback(uint8_t *data, size_t len, uint32_t src);
static int _rpmsg_virtio_rpmsg_callback(uint8_t *data, size_t len, uint32_t src);


// Globals

#define __section_t(S)          __attribute__((__section__(#S)))
#define __resource              __section_t(.resource_table)

const struct _shared_resource_table __resource __attribute__((used)) resource_table = {
	.version = 1,
	.num = 1,
	.reserved = {0, 0},
	.offset = {
		offsetof(struct _shared_resource_table, vdev),
		offsetof(struct _shared_resource_table, cm_trace),
	},
	.vdev= {
		.type = RSC_VDEV, .id = VIRTIO_ID_RPMSG_, .notifyid = 0, .dfeatures = RPMSG_IPU_C0_FEATURES, .gfeatures = 0, .config_len = 0, .status = 0,
		.num_of_vrings = VRING_COUNT, .reserved = {0, 0},
	},
	.vrings = {
			{.da = (struct _vring*)-1, .align = VRING_ALIGNMENT, .num = VRING_NUM_BUFFS, .notifyid = VRING_TX, .reserved = 0},
			{.da = (struct _vring*)-1, .align = VRING_ALIGNMENT, .num = VRING_NUM_BUFFS, .notifyid = VRING_RX, .reserved = 0},
	},
} ;

uint16_t vq_available_idx[VRING_COUNT] = {0, 0};


struct _rpmsg_endpoint rpmsg_ep = {
	.name = RPMSG_SERVICE_NAME,
	.addr = RPMSG_RESERVED_ADDRESSES + 0,
	.dest_addr = RPMSG_ADDR_ANY,
	.cb = _rpmsg_virtio_rpmsg_callback,

};
struct _rpmsg_endpoint rpmsg_ns_ep = {
	.name = "NS",
	.addr = RPMSG_NS_EPT_ADDR,
	.dest_addr = RPMSG_NS_EPT_ADDR,
	.cb = _rpmsg_virtio_ns_callback,
};


// Implementation

void ipcc_init(void) {
	(RCC->MC_AHB3ENSETR = RCC_MC_AHB3ENSETR_IPCCEN);
	uint32_t prioritygroup = NVIC_GetPriorityGrouping();
	NVIC_SetPriority(IPCC_RX1_IRQn, NVIC_EncodePriority(prioritygroup, DEFAULT_IRQ_PRIO, 0));

	NVIC_EnableIRQ(IPCC_RX1_IRQn);

	IPCC_C2->CR  = 0x00000000U;
	IPCC_C2->MR  = (IPCC_ALL_TX_BUF | IPCC_ALL_RX_BUF);
	IPCC_C2->SCR = IPCC_ALL_RX_BUF;
	IPCC_C2->CR |= (IPCC_CR_RXOIE | IPCC_CR_TXFIE);

	IPCC_C2->MR &= ~(IPCC_MR_CH1OM_Msk << IPCC_CHANNEL_1);
	IPCC_C2->MR &= ~(IPCC_MR_CH1OM_Msk << IPCC_CHANNEL_2);

	while ((resource_table.vdev.status & VIRTIO_CONFIG_STATUS_DRIVER_OK) == 0) {}

	resource_table.vrings[0].da->used.flags |= VRING_USED_F_NO_NOTIFY;
}

DECL_ARMCM_IRQ(IPCC_RX1_IRQHandler, IPCC_RX1_IRQn);

void rpmsg_ns_create(void) {
	struct rpmsg_ns_msg ns_msg = {
			.name = RPMSG_SERVICE_NAME,
			.addr = rpmsg_ep.addr,
			.flags = RPMSG_NS_CREATE,
	};

	if(_rpmsg_virtio_send_offchannel_raw(rpmsg_ep.addr,
					RPMSG_NS_EPT_ADDR,
					&ns_msg, sizeof(ns_msg)) < 0) {
		shutdown("Send failed");
	}
}

void IPCC_RX1_IRQHandler(void)
{
	const uint32_t irqmask = (~(IPCC_C2->MR) & IPCC_ALL_RX_BUF) & IPCC_C1->SR;
	IPCC_C2->MR |= irqmask;

	if (irqmask & (1UL << VRING_RX)) {
		_rpmsg_virtio_rx_callback();
		IPCC_C2->SCR |= IPCC_SCR_CH1C << VRING_RX;
		IPCC_C2->MR &= ~(IPCC_MR_CH1OM_Msk << VRING_RX);
	}

	if (irqmask & (1UL << VRING_TX)) {
		IPCC_C2->SCR |= IPCC_SCR_CH1C << VRING_TX;
		IPCC_C2->MR &= ~(IPCC_MR_CH1OM_Msk << VRING_TX);
	}
}

void _virtqueue_kick(size_t vq_idx)
{
	__atomic_thread_fence(__ATOMIC_SEQ_CST);
	if ((resource_table.vrings[vq_idx].da->avail.flags & VRING_AVAIL_F_NO_INTERRUPT) == 0) {
		while (((IPCC_C2->SR) & (IPCC_SR_CH1F_Msk << vq_idx)) != 0) {}
		IPCC_C2->SCR |= IPCC_SCR_CH1S << vq_idx;
	}
}

volatile struct rpmsg_buffer *_virtqueue_get_available_buffer(size_t vq_idx, uint16_t *avail_idx)
{
	__atomic_thread_fence(__ATOMIC_SEQ_CST);

	if (vq_available_idx[vq_idx] == resource_table.vrings[vq_idx].da->avail.idx) {
		return NULL;
	}

	uint16_t head_idx = vq_available_idx[vq_idx]++ & (VRING_NUM_BUFFS - 1);
	*avail_idx = resource_table.vrings[vq_idx].da->avail.ring[head_idx];

	return (struct rpmsg_buffer *)(uint32_t)resource_table.vrings[vq_idx].da->desc[*avail_idx].addr;
}

volatile struct rpmsg_buffer *_rpmsg_virtio_get_tx_payload_buffer(uint16_t*  idx, uint32_t *len)
{
	volatile struct rpmsg_buffer * rp_buffer = NULL;
	int tick_count;

	if (!(resource_table.vdev.status & VIRTIO_CONFIG_STATUS_DRIVER_OK))
		return NULL;

	for(tick_count = RPMSG_TICK_COUNT / RPMSG_TICKS_PER_INTERVAL; !rp_buffer && tick_count; --tick_count) {
		rp_buffer = _virtqueue_get_available_buffer(VRING_TX, idx);
		/**
		 * Here should be delay for RPMSG_TICKS_PER_INTERVAL but this may cause issues with printing.
		 * In normal operation it will never happen.
		 */
	}

	if (!rp_buffer)
		return NULL;

	*len = resource_table.vrings[VRING_TX].da->desc[*idx].len - sizeof(struct rpmsg_hdr);
	return rp_buffer;
}

void _virtqueue_add_consumed_buffer(size_t vq_idx, uint16_t head_idx) {
	volatile struct _vring_used_elem *used_desc = NULL;
	uint16_t used_idx;

	used_idx = resource_table.vrings[vq_idx].da->used.idx & (VRING_NUM_BUFFS - 1);
	used_desc = &resource_table.vrings[vq_idx].da->used.ring[used_idx];
	used_desc->id = head_idx;
	used_desc->len = resource_table.vrings[vq_idx].da->desc[head_idx].len;

	__atomic_thread_fence(__ATOMIC_SEQ_CST);
	resource_table.vrings[vq_idx].da->used.idx++;
}

int _rpmsg_virtio_send_offchannel_raw(uint32_t src, uint32_t dst, const void *data, int len) {
	uint32_t buff_len;
	uint16_t idx;
	volatile struct rpmsg_buffer *buffer = _rpmsg_virtio_get_tx_payload_buffer(&idx, &buff_len);
	if (!buffer) {
		shutdown("Failed to get buffer");
	}

	if (len > (int)buff_len) {
		len = buff_len;
	}

	memcpy(buffer->data, data, len);

	buffer->header.dst = dst;
	buffer->header.src = src;
	buffer->header.len = len;
	buffer->header.reserved = 0;
	buffer->header.flags = 0;

	__atomic_thread_fence(__ATOMIC_SEQ_CST);

	_virtqueue_add_consumed_buffer( VRING_TX, idx);
	_virtqueue_kick(VRING_TX);

	return len;
}

static struct _rpmsg_endpoint *_rpmsg_get_endpoint_by_addr(uint32_t addr) {
	struct _rpmsg_endpoint * ep1 = &rpmsg_ep;
	struct _rpmsg_endpoint * ep2 = &rpmsg_ns_ep;
	if (rpmsg_ep.addr == addr) {
		return ep1;
	}

	if (rpmsg_ns_ep.addr == addr) {
		return ep2;
	}

	return NULL;
}

static void _rpmsg_virtio_rx_callback(void)
{
	struct _rpmsg_endpoint *ept;
	volatile struct rpmsg_buffer * rp_buffer;
	uint16_t idx;
	int status;

	rp_buffer = _virtqueue_get_available_buffer(VRING_RX, &idx);

	while (rp_buffer) {
		ept = _rpmsg_get_endpoint_by_addr(rp_buffer->header.dst);

		if (ept) {
			if (ept->dest_addr == RPMSG_ADDR_ANY) {
				ept->dest_addr = rp_buffer->header.src;
			}

			status = ept->cb( rp_buffer->data,
					rp_buffer->header.len, rp_buffer->header.src);

			if (status < 0) {
				shutdown("Endpoint callback failed");
			}
		}

		if (!(rp_buffer->header.reserved & RPMSG_BUF_HELD)) {
			_virtqueue_add_consumed_buffer(VRING_RX, idx);
		}

		rp_buffer = _virtqueue_get_available_buffer(VRING_RX, &idx);
	}
	_virtqueue_kick(VRING_RX);
}

static struct _rpmsg_endpoint *_rpmsg_get_endpoint_by_name(const char *name, uint32_t dest_addr) {
	if (strncmp(RPMSG_SERVICE_NAME, name, RPMSG_NAME_SIZE)) {
		if (rpmsg_ep.dest_addr == RPMSG_ADDR_ANY || rpmsg_ep.dest_addr == dest_addr) {
			return &rpmsg_ep;
		}
	}

	if (strncmp(RPMSG_NS_NAME, name, RPMSG_NAME_SIZE)) {
		if (rpmsg_ns_ep.dest_addr == RPMSG_ADDR_ANY || rpmsg_ns_ep.dest_addr == dest_addr) {
			return &rpmsg_ns_ep;
		}
	}


	return NULL;
}

static int _rpmsg_virtio_ns_callback(uint8_t *data, size_t len, uint32_t src)
{
	struct _rpmsg_endpoint *ept;
	struct rpmsg_ns_msg *ns_msg;
	uint32_t dest;

	(void)src;

	ns_msg = data;
	if (len != sizeof(*ns_msg)) {
		return 0;
	}

	dest = ns_msg->addr;
	ept = _rpmsg_get_endpoint_by_name(ns_msg->name, dest);
	if (ept) {
		if (ns_msg->flags & RPMSG_NS_DESTROY) {
			ept->dest_addr = RPMSG_ADDR_ANY;
		} else {
			ept->dest_addr = dest;
		}
	}

	return 0;
}


// Integration
#define MAX_TX_BUF 256
static uint8_t tx_buf[MAX_TX_BUF];

static int _rpmsg_virtio_rpmsg_callback(uint8_t *data, size_t len, uint32_t src) {
	size_t pos = 0;
	uint_fast8_t pop_count = 0;
	int_fast8_t ret = 0;

	for (; pos < len;) {
		size_t tx_len = len - pos;
		if (tx_len > 255) {
			tx_len = 255;
		}
		ret = command_find_and_dispatch(data + pos, tx_len, &pop_count);
		if (ret) {
			pos += pop_count;
		} else {
			break;
		}
	}
	return 0;
}

void console_sendf(const struct command_encoder *ce, va_list args)
{
    // Verify space for message
    uint_fast8_t max_size = READP(ce->max_size);
    if (max_size > sizeof(tx_buf))
        // Not enough space for message
        return;

    // Generate message
    uint_fast8_t msglen = command_encode_and_frame(tx_buf, ce, args);

	_rpmsg_virtio_send_offchannel_raw(rpmsg_ep.addr, rpmsg_ep.dest_addr, tx_buf, msglen);
}

