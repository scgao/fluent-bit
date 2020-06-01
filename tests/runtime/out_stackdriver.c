#include <fluent-bit.h>
#include "flb_tests_runtime.h"


#define JSON_ES	"["		\
	"1590727825,"			\
	"{"				\
	"\"key_0\": false,"		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define JSON_VALID_OPERATION	"["		\
	"1590727825,"			\
	"{"				\
	"\"logging.googleapis.com/operation\": {\"id\":\"test_id\",\"producer\":\"pro\", \"first\":true,\"last\":false},"		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

void flb_test_stackdriver_intail(void);
void flb_test_stackdriver_in_lib(void);
void flb_test_stackdriver_valid_operation(void);

/* Test list */
TEST_LIST = {
    {"operation_in_tail", flb_test_stackdriver_intail},
    {"operation_in_lib",  flb_test_stackdriver_in_lib},
    {"operation_valid_operation",  flb_test_stackdriver_valid_operation},
    {NULL, NULL}
};

void flb_test_stackdriver_intail(void) {
    int ret;
    int size = sizeof(JSON_ES) - 1;
    flb_ctx_t *ctx;
    int in_ffd;
    int out_ffd;

    ctx = flb_create();
    TEST_CHECK(ctx != NULL);
    flb_service_set(ctx, "Flush", "1", "Daemon", "off", "Log_Level", "info","Parsers_File", "parser.conf", NULL);

    in_ffd = flb_input(ctx, (char *) "tail", NULL);
    TEST_CHECK(in_ffd >= 0);
    flb_input_set(ctx, in_ffd, "tag", "multi-line-bit","Path","/tmp/fb_text.log","Parser","JSON_fmt", NULL);

    out_ffd = flb_output(ctx, (char *) "stackdriver", NULL);
    TEST_CHECK(out_ffd >= 0);
    flb_output_set(ctx, out_ffd, "Match", "*", "resource", "gce_instance", NULL);

    ret = flb_start(ctx);
    TEST_CHECK(ret == 0);

    /*flb_lib_push(ctx, in_ffd, (char *) JSON_ES, size);*/
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void flb_test_stackdriver_in_lib(void) {
    int ret;
    int size = sizeof(JSON_ES) - 1;
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

    flb_lib_push(ctx, in_ffd, (char *) JSON_ES, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

void flb_test_stackdriver_valid_operation(void) {
    int ret;
    int size = sizeof(JSON_VALID_OPERATION) - 1;
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

    flb_lib_push(ctx, in_ffd, (char *) JSON_VALID_OPERATION, size);
    sleep(1); /* waiting flush */

    flb_stop(ctx);
    flb_destroy(ctx);
}

