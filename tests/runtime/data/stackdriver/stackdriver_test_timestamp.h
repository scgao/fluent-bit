/* 2020-07-21T16:40:42.000012345Z */
#define TIMESTAMP_FORMAT_OBJECT_COMMON_CASE	"["		\
	"1591111124,"			\
	"{"				\
        "\"timestamp\": "		\
        "{"            \
            "\"seconds\": \"1595349642\","          \
            "\"nanos\": \"12345\""      \
        "}"     \
	"}]"

/* Default: 2020-07-21T16:40:00Z */
#define TIMESTAMP_FORMAT_OBJECT_NOT_A_MAP	"["		\
	"1595349600,"			\
	"{"				\
        "\"timestamp\": \"string\""	     \
	"}]"

/* Default: 2020-07-21T16:40:00Z */
#define TIMESTAMP_FORMAT_OBJECT_MISSING_SUBFIELD	"["		\
	"1595349600,"			\
	"{"				\
        "\"timestamp\": "		\
        "{"            \
            "\"nanos\": \"12345\""      \
        "}"     \
	"}]"

/* Default: 2020-07-21T16:40:00Z */
#define TIMESTAMP_FORMAT_OBJECT_INCORRECT_TYPE_SUBFIELDS	"["		\
	"1595349600,"			\
	"{"				\
        "\"timestamp\": "		\
        "{"            \
            "\"seconds\": \"string\","          \
            "\"nanos\": true"      \
        "}"     \
	"}]"
