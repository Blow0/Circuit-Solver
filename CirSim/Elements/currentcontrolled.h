#ifndef _CURRENTCONTROLLED_H
#define _CURRENTCONTROLLED_H

class CurrentControlledSource
{
private:
	static unsigned int m_currentControlledCount;

protected://Constructors
	CurrentControlledSource()
	{
		m_currentControlledCount++;
	}

public:
	static unsigned int getCurrentControlledCount() {return m_currentControlledCount; }
};

#endif // !_CURRENTCONTROLLED_H
