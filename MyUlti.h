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

// Dump a string as hex map to screen
int HexDump(const std::string &);

// Convert GetLastError() to message for easy debug
std::string LastErrorMessage();

#endif MY_ULTI_H
