#include "common.h"
#include "main.h"
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

int main() {
	char key;
	std::string player_input;
	std::vector<std::string> args;
	args.reserve(10);
	printIntro();

	World my_world;

	args.push_back("look");

	while (1) {
		if (_kbhit() != 0) {
			key = _getch();
			if (key == '\b') {
				if (player_input.length() > 0) {
					player_input.pop_back();
					std::cout << '\b' << " " << '\b';
				}
			} else if (key != '\r') {
				player_input += key;
				std::cout << key;
			} else {
				Tokenize(player_input, args);
			}
		}

		if (args.size() > 0 && Compare(args[0], "quit")) {
			break;
		}

		if (my_world.Update(args) == false) {
			std::cout << "\nSorry, I do not understand your command.\n";
		}
		if (args.size() > 0) {
			args.clear();
			player_input = "";
			std::cout << "> ";
		}
	}
	std::cout << "\nThanks for playing, Bye!\n";
	return 0;
}

void printIntro() {
	std::cout << "Welcome to Zork clone by Marco Rodriguez\n";
}