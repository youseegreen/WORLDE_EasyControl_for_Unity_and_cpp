#include "MidiControllerAPI.h"

#ifdef DEBUG_MODE

#include <stdio.h>
#include <windows.h>

int main() {

	OpenController();

	int value[9] = { 0 };

	while (true) {
		GetButtonState(value);
		for (int i = 0; i < 9; i++) {
			if (value[i] == -1)printf("undefined ");
			else printf("%d ", value[i]);
		}
		printf("\n");
		Sleep(5);
	}

	return 1;
}

#endif // !DEBUGMODE