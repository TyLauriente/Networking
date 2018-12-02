#pragma once

#include <Network.h>
#include <XEngine.h>

enum class MessageType : uint32_t
{
	Msg_Snapshot,
	Assignment,
	Msg_Create,
	Msg_Update,
	Msg_Destory,
	Msg_Command
};


