#include "endpoint.h"

#include <serializer.h>

using std::move;
using std::vector;

uint32_t Endpoint::size() const { return sizeof(uint16_t) + address.length(); }

vector<uint8_t> Endpoint::toBytes() const {
  vector<uint8_t> bytes;
  bytes.reserve(size());

  vector<uint8_t> addressBytes = Serializer::stringToBytes(address);
  vector<uint8_t> portBytes = Serializer::u16ToBytes(port);

  bytes = move(addressBytes);
  copy(portBytes.begin(), portBytes.end(), back_inserter(bytes));

  return bytes;
}

Endpoint Endpoint::fromBytes(vector<uint8_t> &&bytes) {
  Endpoint endpoint;

  endpoint.address = Serializer::stringFromBytes(bytes);
  endpoint.port = Serializer::u16FromBytes(bytes);

  return endpoint;
}