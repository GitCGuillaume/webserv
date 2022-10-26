#include "Config.hpp"

Config::Config(const std::string &conf) : _is(conf.c_str())
{
}
Config::Config(const Config &src) {}
~Config::Config() {}