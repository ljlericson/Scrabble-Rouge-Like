#pragma once

enum class EventType
{
	noEvent,

	transferToGameLayer,
	transferToMenuLayer,

	roundStart,
	roundEnd,
	wordConfirmed,

	screenShake,

	enterDevMode,

	gameEnd
};