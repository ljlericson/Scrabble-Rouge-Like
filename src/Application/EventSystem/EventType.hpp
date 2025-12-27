#pragma once

enum class EventType
{
	noEvent,

	gameStart,
	gameEnd,
	wordConfirmed,

	shuffleTiles,
	screenShake,

	enterDevMode
};