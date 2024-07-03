#pragma once

#include <json.hpp>
using json = nlohmann::json;

char *wc2c(const wchar_t *);
wchar_t *c2wc(const char *);
