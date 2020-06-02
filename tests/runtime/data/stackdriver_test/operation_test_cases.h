#define EMPTY_PAYLOAD	"["		\
	"1591111124,"			\
	"{"				\
	"}]"

#define EMPTY_PAYLOAD_WITH_VALID_OPERATION	"["		\
	"1591111124,"			\
	"{"				\
        "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"id\": \"ID1\","          \
            "\"producer\": \"PRODUCER1\","      \
            "\"first\": true,"      \
            "\"last\": false"       \
        "}"     \
	"}]"

#define COMMON_PAYLOAD_WITHOUT_OPERATION "["		\
	"1591111124,"			\
	"{"				\
	"\"message\": \"COMMON_PAYLOAD_WITHOUT_OPERATION\","		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define COMMON_PAYLOAD_WITH_VALID_OPERATION "["		\
	"1591111124,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"id\": \"ID1\","          \
            "\"producer\": \"PRODUCER1\","      \
            "\"first\": true,"      \
            "\"last\": false"       \
        "},"     \
	"\"message\": \"COMMON_PAYLOAD_WITH_VALID_OPERATION\","		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define INCORRECT_TYPE_OF_OPERATION "["		\
	"1591111124,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": \"some string\","		\
	"\"message\": \"INCORRECT_TYPE_OF_OPERATION\","		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define MISSING_ID "["		\
	"1591111124,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"producer\": \"PRODUCER1\","      \
            "\"first\": true,"      \
            "\"last\": false"       \
        "},"     \
	"\"message\": \"MISSING_ID\","		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define MISSING_PRODUCER "["		\
	"1591111124,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"id\": \"ID1\","      \
            "\"first\": true,"      \
            "\"last\": false"       \
        "},"     \
	"\"message\": \"MISSING_PRODUCER\","		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define INCORRECT_TYPE_OF_ID "["		\
	"1591111124,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"id\": 123,"          \
            "\"producer\": \"PRODUCER1\","      \
            "\"first\": true,"      \
            "\"last\": false"       \
        "},"     \
	"\"message\": \"INCORRECT_TYPE_OF_ID\","		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define INCORRECT_TYPE_OF_FIRST "["		\
	"1591111124,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"id\": \"ID1\","          \
            "\"producer\": \"PRODUCER1\","      \
            "\"first\": \"some string\","      \
            "\"last\": false"       \
        "},"     \
	"\"message\": \"INCORRECT_TYPE_OF_FIRST\","		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define EXTRA_SUB_FIELD "["		\
	"1591111124,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"id\": \"ID1\","          \
            "\"producer\": \"PRODUCER1\","      \
            "\"first\": true,"      \
            "\"last\": false,"       \
            "\"extra_key1\": \"extra_val1\""       \
        "},"     \
	"\"message\": \"EXTRA_SUB_FIELD\","		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"
