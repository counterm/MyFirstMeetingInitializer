#pragma once
class CProperty  
{  
private:  
    wstring _value;  
public:  
    CProperty(wstring value);  
	void setValue(wstring);
    wstring getString(void);  
    int getInteger(void);  
    bool getBoolean(void);  
}; 