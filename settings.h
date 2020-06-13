// Update treshold in milliseconds, messages will only be sent on this interval
#define UPDATE_INTERVAL 1000 // 1 second
//#define UPDATE_INTERVAL 10000 // 10 seconds
//#define UPDATE_INTERVAL 60000  // 1 minute
//#define UPDATE_INTERVAL 300000 // 5 minutes

#define HOSTNAME "p1meter"
#define OTA_PASSWORD "admin"

#define BAUD_RATE 115200
#define P1_SERIAL_RX RX
#define P1_MAXLINELENGTH 1050

#define MQTT_MAX_RECONNECT_TRIES 100
#define MQTT_ROOT_TOPIC "sensors/power/p1meter"

#define NUMBER_OF_READOUTS 21

long LAST_RECONNECT_ATTEMPT = 0;
long LAST_UPDATE_SENT = 0;

char WIFI_SSID[32] = "";
char WIFI_PASS[32] = "";

char MQTT_HOST[64] = "";
char MQTT_PORT[6]  = "";
char MQTT_USER[32] = "";
char MQTT_PASS[32] = "";

char telegram[P1_MAXLINELENGTH];

struct TelegramDecodedObject
{
  String name;
  long value;
  char code[16];
  char startChar = '(';
  char endChar = ')';
};

struct TelegramDecodedObject telegramObjects[NUMBER_OF_READOUTS];

// You can turn the CRC on and off here
bool useCRC = true;
unsigned int currentCRC = 0;
