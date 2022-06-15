#include <map>
#include <iostream>
int main(void)
{
	std::map<int, int> mp;


	for (size_t i = 0; i < 500; i++)
	{
		mp.insert(std::make_pair(i, i));
	}
	
	for (std::map<int, int>::iterator it = mp.find(250); it != mp.end(); --it)
	{
		std::cout << it->first << " " << it->second << std::endl;
	}

	for (std::map<int, int>::iterator it = mp.end(); it != --mp.begin(); it--)
	{
		std::cout << it->first << " " << it->second << std::endl;
	}
}