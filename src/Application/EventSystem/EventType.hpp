#pragma once

enum class EventType
{
	// default event
	noEvent,

	// game state changes
	roundStart,
	roundEnd,
	wordConfirmed,
	gameEnd,

	// state changes
	transferToStartState,
	transferToGameState,
	transferToEndState,

	// language switches
	switchTo_USEng_Lang,
	switchTo_AUEng_Lang,
	switchTo_NZEng_Lang,

	// extras
	screenShake,
	enterDevMode
};