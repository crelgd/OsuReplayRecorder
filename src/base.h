#pragma once

#define IFEL(val, msg) { if (!val) std::runtime_error(std::string(msg)); }

