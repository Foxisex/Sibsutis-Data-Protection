#include <libdp/lab5.hpp>

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> distr(4, 10);

	server serv;
	const bigint n = serv.send_n();
	const bigint d = serv.send_d();

	size_t count = distr(gen);
	std::cout << "Количество избирателей: " << count << std::endl;
	std::vector<constituent> cons;

	for(size_t i = 0; i < count; ++i) {
		cons.push_back(constituent(n, d));
	}

	for(size_t j = 0; j < 2; ++j) {
		std::cout << "\nГолосование " << j + 1 << std::endl << std::endl;

		for(size_t i = 0; i < count; ++i) {
			bigint h1 = cons[i].send_h1();
			if(!serv.check_registr(h1)) {
				cons[i].create_s(serv.create_bulletin(h1), n);

				if(serv.check_bulletin(cons[i].send_s(), cons[i].send_m())) {
					std::cout << "Бюллетень избирателя " << i + 1 << " действителен.\n";
				} else {
					std::cout << "AAAAAA! Бюллетень избирателя " << i + 1 << " НЕ действителен.\n";
				}

			} else {
				std::cout << "Ой-ёй! Избиратель " << i + 1 << " уже голосовал!!!\n";
			}
		}
	}

	serv.print_results();


	return 0;
}