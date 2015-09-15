

#include "../KVCpp/KVCpp.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
using namespace KVCpp;
string getstr(ifstream& infile);
int main()
{
	KVParser parser;

	std::ifstream  infile("D:\\npc_abilities_custom.txt");
	string str = getstr(infile);
	
	KVValue v = parser.Parser(str);
	
	//
	KVValue v_ablilitys;
	KVValue v_ablility;
	string cv;
	if (v.HasKey("DOTAAbilities"))
	{
		v_ablilitys = v["DOTAAbilities"];
		 cv=v_ablilitys.Convert2Str();
  	}
	if (v_ablilitys.HasKey("dota_ability_dummy"))
	{
		v_ablility = v_ablilitys["dota_ability_dummy"];

	}

	if (v_ablility.GetType() == VT_OBJ)
	{
		KVValue v_modifiers = v_ablility["Modifiers"];
		if (v_modifiers.GetType()== VT_OBJ)
		{
			KVValue v_modifier;
			KVValue v_modifierStates;
			int count = v_modifiers.GetCount();
			for (int i = 0; i < count; i++)
			{
				v_modifier = v_modifiers[i];
				if (!v_modifier.IsNull())
				{
					v_modifierStates = v_modifier["States"];
					if (!v_modifierStates.IsNull() && v_modifierStates.GetType() == VT_OBJ)
					{
						int count = v_modifierStates.GetCount();

						for (int i = 0; i < count; i++)
						{
							cout << v_modifierStates[i].GetString()<<endl;
						}

					}
					
				}
			

			
			}
		}
		
	}

	int i = 0;
	cin >> i;

	if (i == 1)
	{
		ofstream ofs("D:\\out.txt");
		ofs.write(cv.c_str(), cv.size());
		ofs.close();
	}
	
}


string getstr(ifstream& infile)
{
	istream_iterator<char> begin(infile);
	istream_iterator<char> end;
	vector<char> vec;
	infile>> noskipws;
	copy(begin, end, back_inserter(vec));
	return string(vec.data(), vec.size());
}