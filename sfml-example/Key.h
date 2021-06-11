#pragma once

class Key
{
public:
	bool justPressed();
	bool justRelease();
	bool isPressed();
	bool isRelease();

	int pressCount = 0;
	int releaseCount = 0;
private:

};