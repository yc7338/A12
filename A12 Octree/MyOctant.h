/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/11
----------------------------------------------*/
#ifndef __MYOCTANT_H_
#define __MYOCTANT_H_

#include "RE\ReEng.h"
#include "MyBOClass.h"

//System Class
class MyOctant
{
	uint m_nChildren = 0;
	MyOctant* m_pParent = nullptr;
	MyOctant* m_pChild[8];

	uint m_nLevel;

	float m_fSize = 0.0f; //Size of the octant

	MeshManagerSingleton* m_pMeshMngr = nullptr;//Mesh Manager Singleton
	MyBOClass* m_pBObj = nullptr;
	std::vector<int>collidedIndex = std::vector<int>();

	vector3 m_v3Center = vector3(0.0f); //Will store the center point of the Object Class

	std::vector<MyBOClass*> objectList;
public:
	/*
	USAGE: Constructor
	ARGUMENTS: ---
	OUTPUT: class object
	*/
	MyOctant(void);

	MyOctant(vector3 center, float size);
	/*
	USAGE: Copy Constructor
	ARGUMENTS: class object to copy
	OUTPUT: class object instance
	*/
	MyOctant(MyOctant const& other);
	/*
	USAGE: Copy Assignment Operator
	ARGUMENTS: class object to copy
	OUTPUT: ---
	*/
	MyOctant& operator=(MyOctant const& other);
	/*
	USAGE: Destructor
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~MyOctant(void);
	/*
	USAGE: Changes object contents for other object's
	ARGUMENTS:
	- MyOctant& other -> object to swap content from
	OUTPUT: ---
	*/
	void Swap(MyOctant& other);
	
	/*
	USAGE: Gets the Octant size
	ARGUMENTS: ---
	OUTPUT: Size of the octant
	*/
	float GetSize(void);
	
	/*
	USAGE:
	ARGUMENTS:
	- vector3 a_v3Color = REDEFAULT -> Color of the Object to display if the value is REDEFAULT it
	-- will display Objects in white and colliding ones in red.
	OUTPUT: ---
	*/
	void DisplayBox(vector3 a_v3Color = REDEFAULT);

	void Subdivide(void);

	MyOctant* GetChild(uint nIndex);

	void SetParent(MyOctant* a_pParent);

	void KillBranch();

	std::vector<MyBOClass*> GetObjects();

	void CheckSubdivision();

	void AddToObjects();

	//only use this for the root class
	void PushToRootObjectList(MyBOClass* objectIn);

	//check parent's list of items to see what goes into it, check subdivision
	void CheckCollision();

	void AddObjectList(std::vector<MyBOClass*> list);

	void DrawObjects();

private:
	/*
	USAGE: Deallocates member fields
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Release(void);
	/*
	USAGE: Allocates member fields
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Init(void);
};

#endif //__MYOCTANT_H__
