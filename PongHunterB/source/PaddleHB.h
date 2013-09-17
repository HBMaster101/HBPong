#pragma once
#ifndef _PADDLEHB_H_
#define _PADDLEHB_H_

struct vector 
{
	float fX;
	float fY;
};
struct moveableObject
{
	vector iPosition;
	vector iSpeed;
	int iSprite;
	int iWidth;
	int iHeight;
};

#endif //_PADDLEHB_H_