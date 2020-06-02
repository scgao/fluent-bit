#include <fluent-bit.h>
#include "flb_tests_runtime.h"

#include "data/stackdriver_test/operation_test_cases.h"
#include "data/stackdriver_test/operation_test_long_payload.h"

void empty_payload(void);
void empty_payload_with_valid_operation(void);
void common_payload_without_operation(void);
void common_payload_with_valid_operation(void);
void long_payload_with_valid_operation(void);
void incorrect_type_of_operation(void);
void missing_id(void);
void missing_producer(void);
void incorrect_type_of_id(void);
void incorrect_type_of_first(void);
void extra_sub_field(void);

/* Test list */
TEST_LIST = {
    {"The payload is empty: ", empty_payload},
    {"The payload only has a valid operation field: ",  empty_payload_with_valid_operation},
    {"Common payload without operation: ",  common_payload_without_operation},
    {"Common payload with valid operation: ",  common_payload_with_valid_operation},
    {"Long payload with valid operation: ",  long_payload_with_valid_operation},
    {"Incorrect type of operation: ",  incorrect_type_of_operation},
    {"Missing id: ",  missing_id},
    {"Missing producer: ",  missing_producer},
    {"Incorrect type of id: ",  incorrect_type_of_id},
    {"Incorrect type of first: ",  incorrect_type_of_first},
    {"Extra sub-field: ",  extra_sub_field},
    {NULL, NULL}
};


void empty_payload(void) {
    int ret;
    int size = sizeof(EMPTY_PAYLOAD) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info", NULL);

    in_ffd = flb_input(ctx, (char *) "lib", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_lib_push(ctx, in_ffd, (char *) EMPTY_PAYLOAD, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void empty_payload_with_valid_operation(void) {
    int ret;
    int size = sizeof(EMPTY_PAYLOAD_WITH_VALID_OPERATION) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info", NULL);

    in_ffd = flb_input(ctx, (char *) "lib", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_lib_push(ctx, in_ffd, (char *) EMPTY_PAYLOAD_WITH_VALID_OPERATION, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void common_payload_without_operation(void) {
    int ret;
    int size = sizeof(COMMON_PAYLOAD_WITHOUT_OPERATION) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info", NULL);

    in_ffd = flb_input(ctx, (char *) "lib", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_lib_push(ctx, in_ffd, (char *) COMMON_PAYLOAD_WITHOUT_OPERATION, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void common_payload_with_valid_operation(void) {
    int ret;
    int size = sizeof(COMMON_PAYLOAD_WITH_VALID_OPERATION) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info", NULL);

    in_ffd = flb_input(ctx, (char *) "lib", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_lib_push(ctx, in_ffd, (char *) COMMON_PAYLOAD_WITH_VALID_OPERATION, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void long_payload_with_valid_operation(void) {
    int ret;
    int size = sizeof(LONG_PAYLOAD_WITH_VALID_OPERATION) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info", NULL);

    in_ffd = flb_input(ctx, (char *) "lib", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_lib_push(ctx, in_ffd, (char *) LONG_PAYLOAD_WITH_VALID_OPERATION, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void incorrect_type_of_operation(void) {
    int ret;
    int size = sizeof(INCORRECT_TYPE_OF_OPERATION) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info", NULL);

    in_ffd = flb_input(ctx, (char *) "lib", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_lib_push(ctx, in_ffd, (char *) INCORRECT_TYPE_OF_OPERATION, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void missing_id(void) {
    int ret;
    int size = sizeof(MISSING_ID) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info", NULL);

    in_ffd = flb_input(ctx, (char *) "lib", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_lib_push(ctx, in_ffd, (char *) MISSING_ID, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void missing_producer(void) {
    int ret;
    int size = sizeof(MISSING_PRODUCER) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info", NULL);

    in_ffd = flb_input(ctx, (char *) "lib", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_lib_push(ctx, in_ffd, (char *) MISSING_PRODUCER, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void incorrect_type_of_id(void) {
    int ret;
    int size = sizeof(INCORRECT_TYPE_OF_ID) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info", NULL);

    in_ffd = flb_input(ctx, (char *) "lib", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_lib_push(ctx, in_ffd, (char *) INCORRECT_TYPE_OF_ID, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void incorrect_type_of_first(void) {
    int ret;
    int size = sizeof(INCORRECT_TYPE_OF_FIRST) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info", NULL);

    in_ffd = flb_input(ctx, (char *) "lib", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_lib_push(ctx, in_ffd, (char *) INCORRECT_TYPE_OF_FIRST, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void extra_sub_field(void) {
    int ret;
    int size = sizeof(EXTRA_SUB_FIELD) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info", NULL);

    in_ffd = flb_input(ctx, (char *) "lib", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    flb_lib_push(ctx, in_ffd, (char *) EXTRA_SUB_FIELD, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}



