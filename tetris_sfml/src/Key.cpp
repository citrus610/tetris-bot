#include "Key.h"

bool Key::justPressed()
{
	return this->pressCount == 1;
}

bool Key::justRelease()
{
	return this->releaseCount == 1;
}

bool Key::isPressed()
{
	return this->pressCount > 0;
}

bool Key::isRelease()
{
	return this->releaseCount > 0;
}
