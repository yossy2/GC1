#pragma once
// ƒŒƒCƒ„[
// ”š‚ªá‚¢‚Ù‚¤‚ª‰œ
enum class LAYER
{
	BG,
	CHAR,
	UI,
	MAX
};

LAYER begin(LAYER);
LAYER end(LAYER);
LAYER operator++(LAYER& id);
LAYER operator*(LAYER id);