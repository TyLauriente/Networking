#pragma once

/*
	Filename:		Common.h
	Created by:		Peter Chan
*/

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>