#include <libdp/lab3.hpp>

int main()
{
	std::string filename = "testhren.c";
	std::string filename2 = "testhren2.c";
	std::string filename3 = "testhren3.c";
	// std::string filename = "2022-03-12-134104.jpg";
	auto size = std::filesystem::file_size(filename);
	std::string data = read_file(filename);
	std::string data2 = read_file(filename2);
	std::string data3 = read_file(filename3);

	auto dN = RSA_sign(filename, data);
	auto valid = RSA_valid(filename, dN[0], dN[1], size);
	
	auto Pgy = Elgamal_sign(filename2, data2);
	auto valid2 = Elgamal_valid(filename2, Pgy, size);

	auto pqa = GOST_sign(filename3, data3);
	auto valid3 = GOST_valid(filename3, pqa, size);
	if(valid) {
		std::cout << "Подпись класс" << std::endl;
	} else {
		std::cout << "Ерунда  вышла" << std::endl;
	}
	if(valid2) {
		std::cout << "Подпись класс" << std::endl;
	} else {
		std::cout << "Ерунда  вышла" << std::endl;
	}

if(valid3) {
		std::cout << "Подпись класс" << std::endl;
	} else {
		std::cout << "Ерунда  вышла" << std::endl;
	}

	return 0;
}