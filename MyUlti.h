#pragma once
#ifndef MY_ULTI_H
#define MY_ULTI_H

#include <string>

// Decode a base64 string
std::string B64Decode(const std::string &);

// Get $HOMEPATH of current user
std::string GetUserHomepath();

// Load an array of bytes into string
std::string FromCharArray(const char *, const int &);

#endif MY_ULTI_H
