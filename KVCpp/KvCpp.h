

#ifndef __KVCPP__
#define __KVCPP__
//STL headers
#include <string>
#include <map>
#include <vector>
using std::map;
using std::string;
using std::vector;


namespace KVCpp
{
	//forward declare
	class KVValue;

	enum ValueType
	{
		VT_STR,
		VT_OBJ,
		VT_NONE
	};

	class KVParser
	{
	public:
		//constructor & destructor
		KVParser();
		~KVParser();
		//
		KVValue Parser(string str);
		KVValue Parser(const char*);
		
	private:

		bool _readKeyValue(string &, string&, string&);
	};
    

	
	class KVValue
	{
	public:
		//constructor & destructor
		
		KVValue();
		~KVValue();
		

		//methods
		int GetCount();   //get key count
		void Insert(string strKey,KVValue value);
		bool HasKey(string key);
		string Convert2Str(int depth=0);  //将value装换为string,便于保存，丢失comment信息

		bool IsNull()
		{
			return (_type == VT_NONE) ? true : false;
		};


		//inline functions
		void SetStringValue(string str)  
		{
			_strValue = str;
		}
		void SetType(ValueType type)
		{
			_type = type;
		}
		ValueType GetType()
		{

			return _type;
		}
		string  GetString()
		{
			return _strValue;
		};
	    //operations

		KVValue operator[](string key);
		KVValue operator[](unsigned key);
	private:
		map < string, KVValue> _kvMap;
		ValueType _type; // to mark this value a string or obj
		string _strValue; //string value

		vector<string>  _vecKeys;
	};


}
#endif