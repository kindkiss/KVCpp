


//KcCpp headers
#include "KvCpp.h"

using namespace KVCpp;


KVParser::KVParser()
{

}

KVParser::~KVParser()
{

}
KVValue KVParser::Parser(const char* pc)
{
	string str(pc);
	return Parser(pc);
}
KVValue KVParser::Parser(string str)
{

	KVValue value;
	//process parser
	string strKey;
	string strValue;
	while (_readKeyValue(str, strKey, strValue))
	{
		KVValue tempValue;

		if (strValue != ""&&strKey != "")
		{
			if (strValue.size() > 0)
			{
				if (strValue[0] == '\"')
				{
					strValue = strValue.substr(1);
					tempValue.SetStringValue(strValue);
					tempValue.SetType(VT_STR);
				}
				else
				{
					strValue = strValue.substr(1);
					tempValue = Parser(strValue);
					tempValue.SetType(VT_OBJ);
				}

			}
			
			value.Insert(strKey, tempValue);
		}
	}

	//
	return value;

}


bool KVParser::_readKeyValue(string &strCurStr, string& strKey, string& strValue)
{
		strKey = "";
		strValue = "";
		int index = 0;
		int maxIndex = strCurStr.size();
		while (strCurStr[index] == ' ' || strCurStr[index] == '\n' || strCurStr[index] == '\t')
		{
			index++; //获取第一个不为空格的字符索引
			if (index >= maxIndex)
				return false;
		}
		char ch = strCurStr[index];

		switch (ch)
		{

		case '\"':{  //如果是双引号 就读取key
					  index++;
					  while (strCurStr[index] != '\"')
					  {
						  strKey.push_back(strCurStr[index]);

						  index++;
						  if (index >= maxIndex)
						  {
							  return false;
						  }

					  }

		}; break;
		case '/':{  //如果是单斜杆，就判断下一个字符
					 if ((index < maxIndex - 1) && (strCurStr[index + 1] == '/'))
					 {
						 index++;
						 if (index + 1 >= maxIndex)
							 return false;
						 while (strCurStr[++index] != '\n');
						 index++;
						 if (index >= maxIndex)
						 {
							 return false;
						 }
						 string newStr = strCurStr.substr(index);

						 if (!_readKeyValue(newStr, strKey, strValue))
							 return false;
						 else
						 {
							 //推进strCurStr
							 if (index + 1 < maxIndex)
							 {
								 strCurStr = newStr;
							 }
							 else
							 {
								 strCurStr = "";
							 }
							 return true;
						 }
							 
					 }
					 else
					 {
						 return false;
					 }

		}; break;

		default: return false;
		}
		//read value string
		index++;  //跳过关键字后面的双引号
		if (index >= maxIndex)
		{
			return false;
		}
		while (strCurStr[index] == ' ' || strCurStr[index] == '\n' || strCurStr[index] == '\t')
		{
			index++;//跳过空格和换行符
			if (index >= maxIndex)
			{
				return false;
			}
		}

		ch = strCurStr[index];
		switch (ch)
		{
		case '\"':{//如果是引号，代表value是 VT_STR类型（字符串类型）

					  strValue.push_back(ch);//保留首字符，来标记值类型
					  index++;
					  if (index >= maxIndex)
					  {
						  return false;
					  }
					  while ((ch = strCurStr[index]) != '\"') //value 要保留前面的 双引号 或者 左括号 来区分值得类型
					  {

						  strValue.push_back(ch);

						  index++;
						  if (index >= maxIndex)
						  {
							  return false;
						  }

					  }
					  //推进strCurStr
					  if (index + 1 < maxIndex)
					  {
						  strCurStr = strCurStr.substr(++index);
					  }
					  else
					  {
						  strCurStr = "";
					  }

		}; break;

		case '{':{//如果是左括号，代表对象类型
					 strValue.push_back(ch);//保留首字符，来标记值类型
					 int single = 0;
					 index++;
					 if (index >= maxIndex)
					 {
						 return false;
					 }
					 while ((ch = strCurStr[index]) != '}'||single!=0)
					 {
						 //用信号量来匹配 {}，当single=0时
						 if (ch == '{')
							 single++;
						 if (ch == '}')
							 single--;

						 strValue.push_back(ch);

						 ++index;
						 if (index >= maxIndex)
						 {
							 return false;
						 }
					 }
					 //推进strCurStr
					 if (index + 1 < maxIndex)
					 {
						 strCurStr = strCurStr.substr(++index);
					 }
					 else
					 {
						 strCurStr = "";
					 }

		}; break;

		}

		return true;

}


///////////////////////////////////////////////KVVALUE///////////////////////////////////////////

KVValue::KVValue()
{
	_type = VT_NONE;
}
KVValue::~KVValue()
{

}

int KVValue::GetCount()
{
	return _kvMap.size();
}

void KVValue::Insert(string strKey, KVValue value)
{
	_vecKeys.push_back(strKey);
	_kvMap[strKey] = value;
}

KVValue  KVValue::operator[](string key)  //automaticly insert the key_value pair when it mismatch the keys
{
	if (HasKey(key))
	{
		_vecKeys.push_back(key);
	}
	return _kvMap[key];
}
KVValue  KVValue::operator[](unsigned index)
{
	
	if (index < _vecKeys.size())
	{
		string strkey = _vecKeys[index];

		return operator[](strkey);
	}
	

	return KVValue();

}

bool  KVValue::HasKey(string key)
{
	map<string, KVValue>::iterator it_res;
	map<string, KVValue>::iterator it_end = _kvMap.end();

	it_res = _kvMap.find(key);

	if (it_res == it_end)
	{
		return false;
	}

	return true;
	
}

string  KVValue::Convert2Str(int depth)
{
	string strResult;
	if (_type == VT_OBJ)  //OBJ类型的Value
	{
		if (depth != 0)
		{
			strResult.push_back('\n');
		}
		
		for (int i = 0; i < depth; i++)
		{
			strResult.append("\t");
		}
		strResult.push_back('{');
		strResult.push_back('\n');

		int nCount = GetCount();

		string strKey;
		string strValue;
		KVValue value;
		for (int i = 0; i < nCount; i++)
		{
			strKey = _vecKeys[i];
			value = operator[](i); 
		
		
			strValue = value.Convert2Str(depth+1);  //include "" or {}
		

			for (int i = 0; i < depth; i++)
			{
				strResult.append("\t");
			}
			strResult.push_back(' ');
			strResult.push_back('\"');
			strResult.append(strKey);
			strResult.push_back('\"');
			strResult.push_back('\t');
			strResult.push_back('\t');
			strResult.append(strValue);
			strResult.push_back('\n');
			


		}
		if (depth != 0)
		{
			strResult.push_back('\n');
		}
		for (int i = 0; i < depth; i++)
		{
			strResult.append("\t");
		}
		strResult.push_back('}');
		strResult.push_back('\n');
	}
	else //STR类型的Value
	{
		strResult.push_back('\"');
		strResult.append(GetString());
		strResult.push_back('\"');

	}


	return strResult;
}