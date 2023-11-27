#include <libdp/lab4.hpp>

int main()
{
	try{
		Poker poker(4, 52);
		poker.Game();
	} catch (char const * err) {
		std::cout << err<<"\nЁЛЫ-ПАЛЫ(((((" << std::endl;
	}



	return 0;
}