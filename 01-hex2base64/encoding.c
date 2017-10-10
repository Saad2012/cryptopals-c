#include "encoding.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define hex2nibble(x) (isdigit(x) ? x-'0' : isupper(x) ? x-'A'+10 : x-'a'+10)

ByteString * decodeHex(char *encodedString) {
  size_t numBytes = strlen(encodedString) / 2;
  
  ByteString *rawBytes = (ByteString *) malloc(sizeof(ByteString) + numBytes);
  rawBytes->length = numBytes;

  for (size_t i = 0; i < numBytes; ++i) {
    byte_t curByte = hex2nibble(encodedString[i*2]) << 4;
    curByte |= hex2nibble(encodedString[i*2 + 1]);

    rawBytes->bytes[i] = curByte;
  }

  return rawBytes;
}

const char base64chars[] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

char * encodeBase64(ByteString *rawBytes) {
  size_t stringLen = (rawBytes->length / 3) * 4;

  char *encodedString = (char *) malloc(stringLen + 1);
  encodedString[stringLen] = '\0';

  for (size_t i = 0; i < rawBytes->length / 3; i++) {
    byte_t curIndex = rawBytes->bytes[i*3] >> 2;
    encodedString[i*4] = base64chars[curIndex];

    curIndex = (rawBytes->bytes[i*3] << 4) & 0x30;
    curIndex |= (rawBytes->bytes[i*3 + 1] >> 4) & 0x0f;
    encodedString[i*4 + 1] = base64chars[curIndex];

    curIndex = (rawBytes->bytes[i*3 + 1] << 2) & 0x3c;
    curIndex |= (rawBytes->bytes[i*3 + 2] >> 6) & 0x03;
    encodedString[i*4 + 2] = base64chars[curIndex];

    curIndex = rawBytes->bytes[i*3 + 2] & 0x3f;
    encodedString[i*4 + 3] = base64chars[curIndex];
  }

  return encodedString;
}
