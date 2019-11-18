#pragma once

enum class INPUT_ID:int
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	BTN_1,
	BTN_2,
	BTN_3,
	BTN_4,
	MAX
};

INPUT_ID begin(INPUT_ID);
INPUT_ID end(INPUT_ID);

INPUT_ID operator++(INPUT_ID& id);
INPUT_ID operator*(INPUT_ID id);