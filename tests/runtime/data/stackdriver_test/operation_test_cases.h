#define EMPTY_PAYLOAD	"["		\
	"1591057541,"			\
	"{"				\
	"}]"

#define EMPTY_PAYLOAD_WITH_VALID_OPERATION	"["		\
	"1591057541,"			\
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
	"1591057541,"			\
	"{"				\
	"\"key_0\": false,"		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define COMMON_PAYLOAD_WITH_VALID_OPERATION "["		\
	"1591057541,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"id\": \"ID1\","          \
            "\"producer\": \"PRODUCER1\","      \
            "\"first\": true,"      \
            "\"last\": false"       \
        "}"     \
	"\"key_0\": false,"		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define INCORRECT_TYPE_OF_OPERATION "["		\
	"1591057541,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": \"some string\","		\
	"\"key_0\": false,"		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define MISSING_ID "["		\
	"1591057541,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"producer\": \"PRODUCER1\","      \
            "\"first\": true,"      \
            "\"last\": false"       \
        "}"     \
	"\"key_0\": false,"		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define MISSING_PRODUCER "["		\
	"1591057541,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"id\": \"ID1\","      \
            "\"first\": true,"      \
            "\"last\": false"       \
        "}"     \
	"\"key_0\": false,"		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define INCORRECT_TYPE_OF_ID "["		\
	"1591057541,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"id\": 123,"          \
            "\"producer\": \"PRODUCER1\","      \
            "\"first\": true,"      \
            "\"last\": false"       \
        "}"     \
	"\"key_0\": false,"		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define INCORRECT_TYPE_OF_FIRST "["		\
	"1591057541,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"id\": \"ID1\","          \
            "\"producer\": \"PRODUCER1\","      \
            "\"first\": "some string","      \
            "\"last\": false"       \
        "}"     \
	"\"key_0\": false,"		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"

#define EXTRA_SUB_FIELD "["		\
	"1591057541,"			\
	"{"				\
    "\"logging.googleapis.com/operation\": "		\
        "{"            \
            "\"id\": \"ID1\","          \
            "\"producer\": \"PRODUCER1\","      \
            "\"first\": true,"      \
            "\"last\": false,"       \
            "\"extra_key1\": \"extra_val1\""       \
        "}"     \
	"\"key_0\": false,"		\
	"\"key_1\": true,"		\
	"\"key_2\": \"some string\","		\
	"\"key_3\": 0.12345678,"		\
	"\"key_4\": 5000,"		\
	"\"END_KEY\": \"JSON_END\""		\
	"}]"
