


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
			index++; //��ȡ��һ����Ϊ�ո���ַ�����
			if (index >= maxIndex)
				return false;
		}
		char ch = strCurStr[index];

		switch (ch)
		{

		case '\"':{  //�����˫���� �Ͷ�ȡkey
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
		case '/':{  //����ǵ�б�ˣ����ж���һ���ַ�
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
							 //�ƽ�strCurStr
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
		index++;  //�����ؼ��ֺ����˫����
		if (index >= maxIndex)
		{
			return false;
		}
		while (strCurStr[index] == ' ' || strCurStr[index] == '\n' || strCurStr[index] == '\t')
		{
			index++;//�����ո�ͻ��з�
			if (index >= maxIndex)
			{
				return false;
			}
		}

		ch = strCurStr[index];
		switch (ch)
		{
		case '\"':{//��������ţ�����value�� VT_STR���ͣ��ַ������ͣ�

					  strValue.push_back(ch);//�������ַ��������ֵ����
					  index++;
					  if (index >= maxIndex)
					  {
						  return false;
					  }
					  while ((ch = strCurStr[index]) != '\"') //value Ҫ����ǰ��� ˫���� ���� ������ ������ֵ������
					  {

						  strValue.push_back(ch);

						  index++;
						  if (index >= maxIndex)
						  {
							  return false;
						  }

					  }
					  //�ƽ�strCurStr
					  if (index + 1 < maxIndex)
					  {
						  strCurStr = strCurStr.substr(++index);
					  }
					  else
					  {
						  strCurStr = "";
					  }

		}; break;

		case '{':{//����������ţ������������
					 strValue.push_back(ch);//�������ַ��������ֵ����
					 int single = 0;
					 index++;
					 if (index >= maxIndex)
					 {
						 return false;
					 }
					 while ((ch = strCurStr[index]) != '}'||single!=0)
					 {
						 //���ź�����ƥ�� {}����single=0ʱ
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
					 //�ƽ�strCurStr
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
	if (_type == VT_OBJ)  //OBJ���͵�Value
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
	else //STR���͵�Value
	{
		strResult.push_back('\"');
		strResult.append(GetString());
		strResult.push_back('\"');

	}


	return strResult;
}