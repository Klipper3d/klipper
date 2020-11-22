#ifndef __COMMAND_H
#define __COMMAND_H

#include <stdarg.h> // va_list
#include <stddef.h>
#include <stdint.h> // uint8_t
#include "ctr.h" // DECL_CTR

// Declare a function to run when the specified command is received
#define DECL_COMMAND_FLAGS(FUNC, FLAGS, MSG)                    \
    DECL_CTR("DECL_COMMAND_FLAGS " __stringify(FUNC) " "        \
             __stringify(FLAGS) " " MSG)
#define DECL_COMMAND(FUNC, MSG)                 \
    DECL_COMMAND_FLAGS(FUNC, 0, MSG)

// Flags for command handler declarations.
#define HF_IN_SHUTDOWN   0x01   // Handler can run even when in emergency stop

// Declare a constant exported to the host
#define DECL_CONSTANT(NAME, VALUE)                              \
    DECL_CTR_INT("DECL_CONSTANT " NAME, 1, CTR_INT(VALUE))
#define DECL_CONSTANT_STR(NAME, VALUE)                  \
    DECL_CTR("DECL_CONSTANT_STR " NAME " " VALUE)

// Declare an enumeration
#define DECL_ENUMERATION(ENUM, NAME, VALUE)                             \
    DECL_CTR_INT("DECL_ENUMERATION " ENUM " " NAME, 1, CTR_INT(VALUE))
#define DECL_ENUMERATION_RANGE(ENUM, NAME, VALUE, COUNT)        \
    DECL_CTR_INT("DECL_ENUMERATION_RANGE " ENUM " " NAME,       \
                 2, CTR_INT(VALUE), CTR_INT(COUNT))

// Send an output message (and declare a static message type for it)
#define output(FMT, args...)                    \
    command_sendf(_DECL_OUTPUT(FMT) , ##args )

// Declare a message type and transmit it.
#define sendf(FMT, args...)                     \
    command_sendf(_DECL_ENCODER(FMT) , ##args )

// Shut down the machine (also declares a static string to transmit)
#define shutdown(msg)                           \
    sched_shutdown(_DECL_STATIC_STR(msg))
#define try_shutdown(msg)                       \
    sched_try_shutdown(_DECL_STATIC_STR(msg))

#define MESSAGE_MIN 5
#define MESSAGE_MAX 64
#define MESSAGE_HEADER_SIZE  2
#define MESSAGE_TRAILER_SIZE 3
#define MESSAGE_POS_LEN 0
#define MESSAGE_POS_SEQ 1
#define MESSAGE_TRAILER_CRC  3
#define MESSAGE_TRAILER_SYNC 1
#define MESSAGE_PAYLOAD_MAX (MESSAGE_MAX - MESSAGE_MIN)
#define MESSAGE_SEQ_MASK 0x0f
#define MESSAGE_DEST 0x10
#define MESSAGE_SYNC 0x7E

struct command_encoder {
    uint8_t msg_id, max_size, num_params;
    const uint8_t *param_types;
};
struct command_parser {
    uint8_t msg_id, num_args, flags, num_params;
    const uint8_t *param_types;
    void (*func)(uint32_t *args);
};
enum {
    PT_uint32, PT_int32, PT_uint16, PT_int16, PT_byte,
    PT_string, PT_progmem_buffer, PT_buffer,
};

// command.c
void *command_decode_ptr(uint32_t v);
uint8_t *command_parsef(uint8_t *p, uint8_t *maxend
                        , const struct command_parser *cp, uint32_t *args);
uint_fast8_t command_encodef(uint8_t *buf, const struct command_encoder *ce
                             , va_list args);
void command_add_frame(uint8_t *buf, uint_fast8_t msglen);
uint_fast8_t command_encode_and_frame(
    uint8_t *buf, const struct command_encoder *ce, va_list args);
void command_sendf(const struct command_encoder *ce, ...);
int_fast8_t command_find_block(uint8_t *buf, uint_fast8_t buf_len
                               , uint_fast8_t *pop_count);
void command_dispatch(uint8_t *buf, uint_fast8_t msglen);
void command_send_ack(void);
int_fast8_t command_find_and_dispatch(uint8_t *buf, uint_fast8_t buf_len
                                      , uint_fast8_t *pop_count);

// out/compile_time_request.c (auto generated file)
extern const struct command_parser command_index[];
extern const uint8_t command_index_size;
extern const uint8_t command_identify_data[];
extern const uint32_t command_identify_size;
const struct command_encoder *ctr_lookup_encoder(const char *str);
const struct command_encoder *ctr_lookup_output(const char *str);
uint8_t ctr_lookup_static_string(const char *str);

#define _DECL_ENCODER(FMT) ({                   \
    DECL_CTR("_DECL_ENCODER " FMT);             \
    ctr_lookup_encoder(FMT); })

#define _DECL_OUTPUT(FMT) ({                    \
    DECL_CTR("_DECL_OUTPUT " FMT);              \
    ctr_lookup_output(FMT); })

#define _DECL_STATIC_STR(MSG) ({                \
    DECL_CTR("_DECL_STATIC_STR " MSG);          \
    ctr_lookup_static_string(MSG); })

#endif // command.h
