#pragma once

#include <libdp/lab4.hpp>
#include <iterator>
#include <algorithm>
#include <random>
#include <string>


bigint gen_bits_num(uint16_t bits, bool prime)
{
	std::random_device rd;
	// boost::random::independent_bits_engine<boost::mt19937, 1024, bigint> gen(rd());
	std::mt19937 gen(rd());
	bigint lower = 1;
	for(int i = 0; i < bits-1; i++) {
		lower *= 2;
	}
	bigint upper = 1;
	for(int i = 0; i < bits; i++) {
		upper *= 2;
	}
	upper--;

	boost::random::uniform_int_distribution<bigint> distr(lower,upper);
	bigint num;

	do {
   		num = distr(gen);
   } while(!bm::miller_rabin_test(num, 100) && prime);

	return num;
}

bigint str_to_bigint(std::string number)
{
	bigint result = 0;
	uint64_t order = number.length();
	uint8_t high;

	for(uint64_t i = 0; i < order; ++i) {
		high = number[i] & 0b00001111;
		result += high;
		result *= 10;
	}

	return result / 10;
}

class server
{
	private:
		bigint p, q, n, phi;											//Константы
		bigint c, d;													//Закрытый и открытый ключи

		uint64_t votes_yes, votes_no;									//Подсчёт голосов
		uint64_t corrupted;												//Количество недействительных бюллетеней

		std::vector<bigint> registr;									//Реестр избирателей
		std::vector<bigint> bulletins;									//БД зарегистрированных бюллетеней

	public:
		server()
		{
			votes_yes = votes_no = corrupted = 0;

			do {
				p = gen_bits_num(1024, true);
				q = gen_bits_num(1024, true);
			} while(p == q);

			n = p * q;
			phi = (p - 1) * (q - 1);

			std::random_device rd;
			std::mt19937 gen(rd());
			boost::random::uniform_int_distribution<bigint> distr(2, p-2);

			std::array<bigint,3> EvkRes{};
			do {
				do {
					c = distr(gen);
					EvkRes = Evklid(c, phi);
				} while (EvkRes[0] != 1);
				d = EvkRes[2];
				d > 0 ? d = d : d += phi;
			} while (c*d%phi != 1);
		}																//Генерация констант и ключей
		
		bigint send_n()
		{
			return n;
		}																//Публикация числа N

		bigint send_d()
		{
			return d;
		}																//Публикация открытого ключа

		const std::vector<bigint> &get_bulletins()
		{
			return bulletins;
		}																//Публикация списка бюллетеней

		bool check_registr(const bigint h1)
		{
			if(registr.size() == 0) return false;

			return find(registr.begin(), registr.end(), h1) != registr.end();
		}																//Проверка, не голосовал ли избиратель ранее

		bigint create_bulletin(const bigint h1)
		{
			registr.push_back(h1);

			return mod_pow(h1, c, n);
		}																//Создание бюллетеня
		
		bool check_bulletin(const bigint s, const bigint m)
		{
			std::string hash;
			picosha2::hash256_hex_string(m.str(), hash);

			bigint h = str_to_bigint(hash);

			if(h != mod_pow(s, d, n)) {
				++corrupted;
				return false;
			}

			//Подсчёт голосов
			switch(uint8_t(m)) {
				case 255:
					++votes_yes;
				break;
				case 127:
					++votes_no;
				break;
				default:
					++corrupted;
				break;
			};

			bulletins.push_back(s);
			return true;
		}																//Проверка подписи бюллетеня

		void print_results()
		{
			std::cout << "\nГолосов ЗА: " << votes_yes;
			std::cout << "\nГолосов ПРОТИВ: " << votes_no;
			std::cout << "\nИспорченных бюллетеней: " << corrupted << std::endl;
		}																//Вывести результаты голосования
};

class constituent
{
	private:
		bigint rnd;														//Префикс
		bigint v;															//Голос избирателя
		bigint m;															//m = rnd | v
		bigint r;															//Взаимно простое с n
		bigint inv_r;														//Инверсия r по модулю n
		bigint h, h1;														//Хеш и его инверсия
		bigint s;															//Подпись избирателя

	public:
		constituent(const bigint n, const bigint d)
		{
			rnd = gen_bits_num(512, false);

			v = (rand() % 2) ? 255 : 127;

			m = (rnd << 8) | v;

			std::random_device rd;
			std::mt19937 gen(rd());
			boost::random::uniform_int_distribution<bigint> distr(2, n-2);

			do {
				r = distr(gen);
			} while (Evklid(r, n)[0] != 1);

			std::string hash;

			picosha2::hash256_hex_string(m.str(), hash);

			h = str_to_bigint(hash);

			h1 = h * mod_pow(r, d, n) % n; 
		}																//Генерация чисел избирателя

		void create_s(const bigint bulletin, const bigint n)
		{
			// inv_r = mod_inv(r, n);
			inv_r = Evklid(r, n)[2];
			inv_r < 0 ? inv_r += n : inv_r = inv_r;	

			s = bulletin * inv_r % n;
		}																//Подписание бюллетеня

		bigint send_h1()
		{
			return h1;
		}																//Отправляем серверу число h1 
		bigint send_s()
		{
			return s;
		}																//Отправляем серверу число s
		bigint send_m()
		{
			return m;
		}																//Отправляем серверу число m
};
