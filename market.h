#ifndef MARKET_H
#define MARKET_H
#include<vector>
#include<string>
class market
{
public:
	market(int argc, char** argv);
	void start();
private:
	std::vector<std::string> lines;
};
#endif
