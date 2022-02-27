#include "Bridge_Interface_to_Server.h"

std::deque < ServerData > Bridge_IS::S_OUT = {};
std::deque < ServerData > Bridge_IS::S_IN = {};
std::wstring Bridge_IS::key;