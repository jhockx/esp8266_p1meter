unsigned int CRC16(unsigned int crc, unsigned char *buf, int len)
{
  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (unsigned int)buf[pos];

    for (int i = 8; i != 0; i--)
    {
      if ((crc & 0x0001) != 0)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }
  return crc;
}

bool isNumber(char *res, int len)
{
  for (int i = 0; i < len; i++)
  {
    if (((res[i] < '0') || (res[i] > '9')) && (res[i] != '.' && res[i] != 0))
    {
      return false;
    }
  }
  return true;
}

int findCharInArrayRev(char array[], char c, int len)
{
  for (int i = len - 1; i >= 0; i--)
  {
    if (array[i] == c)
    {
      return i;
    }
  }

  return -1;
}

long getValue(char *buffer, int maxlen, char startchar, char endchar)
{
  int s = findCharInArrayRev(buffer, startchar, maxlen - 2);
  int l = findCharInArrayRev(buffer, endchar, maxlen - 2) - s - 1;

  char res[16];
  memset(res, 0, sizeof(res));

  if (strncpy(res, buffer + s + 1, l))
  {
    if (endchar == '*')
    {
      if (isNumber(res, l))
        return (1000 * atof(res));
    }
    else if (endchar == ')')
    {
      if (isNumber(res, l))
        return atof(res);
    }
  }

  return 0;
}

/**
    Decodes the telegram PER line. Not the complete message.
*/
bool decodeTelegram(int len)
{
  int startChar = findCharInArrayRev(telegram, '/', len);
  int endChar = findCharInArrayRev(telegram, '!', len);
  bool validCRCFound = false;

  for (int cnt = 0; cnt < len; cnt++)
  {
    Serial.print(telegram[cnt]);
  }
  Serial.print("\n");

  if (startChar >= 0)
  {
    // * Start found. Reset CRC calculation
    currentCRC = CRC16(0x0000, (unsigned char *)telegram + startChar, len - startChar);
  }
  else if (endChar >= 0)
  {
    // * Add to crc calc
    currentCRC = CRC16(currentCRC, (unsigned char *)telegram + endChar, 1);

    char messageCRC[5];
    strncpy(messageCRC, telegram + endChar + 1, 4);

    messageCRC[4] = 0;
    validCRCFound = (strtol(messageCRC, NULL, 16) == currentCRC);

    currentCRC = 0;
  }
  else
  {
    currentCRC = CRC16(currentCRC, (unsigned char *)telegram, len);
  }

  // Loops throug all the telegramObjects to find the code in the telegram line
  // If it finds the code the value will be stored in the object so it can later be send to the mqtt broker
  for (int i = 0; i < NUMBER_OF_READOUTS; i++)
  {
    if (strncmp(telegram, telegramObjects[i].code, strlen(telegramObjects[i].code)) == 0)
    {
      telegramObjects[i].value = getValue(telegram, len, telegramObjects[i].startChar, telegramObjects[i].endChar);
    }
  }

  return validCRCFound;
}

void readP1Serial()
{
  if (Serial.available())
  {
    memset(telegram, 0, sizeof(telegram));
    while (Serial.available())
    {
      ESP.wdtDisable();
      // Reads the telegram untill it finds a return character
      // That is after each line in the telegram
      int len = Serial.readBytesUntil('\n', telegram, P1_MAXLINELENGTH);
      ESP.wdtEnable(1);

      telegram[len] = '\n';
      telegram[len + 1] = 0;
      yield();

      bool result = decodeTelegram(len + 1);
      // When the CRC is checked (which is also the end of the telegram),
      // all the data collected will be send to the mqtt broker
      if (useCRC) {
        if (result)
        {
          LAST_UPDATE_SENT = millis();
          sendDataToBroker();
        }
      }
      else
      {
        LAST_UPDATE_SENT = millis();
        sendDataToBroker();
      }
    }
  }
}
