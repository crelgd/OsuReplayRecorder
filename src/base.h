#pragma once

#define IFEL(val, msg) { if (val) throw std::runtime_error(std::string(msg)); }
