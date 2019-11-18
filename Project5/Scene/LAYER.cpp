#include <type_traits>
#include "LAYER.h"

LAYER begin(LAYER)
{
	return LAYER::BG;
}

LAYER end(LAYER)
{
	return LAYER::MAX;
}

LAYER operator++(LAYER & id)
{
	if (id != LAYER::MAX)
	{
		id = static_cast<LAYER>(std::underlying_type<LAYER>::type(id) + 1);
	}
	return id;
}

LAYER operator*(LAYER id)
{
	return id;
}