#include "MyOctant.h"
//  MyOctant
void MyOctant::Init(void)
{
	m_nChildren = 0;

	m_nLevel = 0;

	m_v3Center = vector3(0.0f);
	
	m_fSize = 0.0f;

	m_pMeshMngr = MeshManagerSingleton::GetInstance();

	std::vector<MyBOClass*> objectList = std::vector<MyBOClass*>();

	MyOctant* m_pParent = nullptr;
	
	for (int nChild = 0; nChild < 8; nChild++)
	{
		m_pChild[nChild] = nullptr;
	}
}

MyOctant::MyOctant(vector3 center, float size)
{
	Init();
	m_v3Center = center;
	m_fSize = size;

	std::vector<vector3> maxMin = std::vector<vector3>();

	maxMin.push_back(vector3(size) + center);
	maxMin.push_back(vector3(-size) + center);

	m_pBObj = new MyBOClass(maxMin);
}
void MyOctant::Swap(MyOctant& other)
{
	std::swap(m_nChildren, other.m_nChildren);

	std::swap(m_nLevel, other.m_nLevel);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_fSize, other.m_fSize);

	std::swap(m_pMeshMngr, other.m_pMeshMngr);

	std::swap(m_pBObj, other.m_pBObj);

	std::swap(objectList, other.objectList);
}
void MyOctant::Release(void)
{
	if (m_nLevel == 0)
		KillBranch();
}
//The big 3
MyOctant::MyOctant(void)
{
	Init();
}
MyOctant::MyOctant(MyOctant const& other)
{
	m_nLevel = other.m_nLevel;
	m_nChildren = other.m_nChildren;
	m_v3Center = other.m_v3Center;
	m_fSize = other.m_fSize;

	m_pMeshMngr = other.m_pMeshMngr;

	m_pBObj = other.m_pBObj;
	objectList = other.objectList;
}
MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctant::~MyOctant() { Release(); };
//Accessors
float MyOctant::GetSize(void) { return m_fSize; }
//--- Non Standard Singleton Methods
void MyOctant::DisplayBox(vector3 a_v3Color)
{
	for (int nChild = 0; nChild < m_nChildren; nChild++)
	{
		m_pChild[nChild]->DisplayBox(a_v3Color);
	}

	m_pMeshMngr->AddCubeToQueue(glm::translate(m_v3Center) * glm::scale(vector3(m_fSize * 2.0f)), a_v3Color, WIRE);
}

void MyOctant::Subdivide(void)
{

	m_nChildren = 8;
	int nIndex = 0;
	float nNewSize = m_fSize / 2.0f;

	vector3 vCenter = m_v3Center + vector3(nNewSize, nNewSize, nNewSize);
	m_pChild[nIndex] = new MyOctant(vCenter, nNewSize);
	m_pChild[nIndex]->m_pParent = this;
	m_pChild[nIndex]->m_nLevel = m_nLevel + 1;

	vCenter = m_v3Center + vector3(-nNewSize, nNewSize, nNewSize);
	nIndex++;
	m_pChild[nIndex] = new MyOctant(vCenter, nNewSize);
	m_pChild[nIndex]->m_pParent = this;
	m_pChild[nIndex]->m_nLevel = m_nLevel + 1;

	vCenter = m_v3Center + vector3(-nNewSize, -nNewSize, nNewSize);
	nIndex++;
	m_pChild[nIndex] = new MyOctant(vCenter, nNewSize);
	m_pChild[nIndex]->m_pParent = this;
	m_pChild[nIndex]->m_nLevel = m_nLevel + 1;

	vCenter = m_v3Center + vector3(nNewSize, -nNewSize, nNewSize);
	nIndex++;
	m_pChild[nIndex] = new MyOctant(vCenter, nNewSize);
	m_pChild[nIndex]->m_pParent = this;
	m_pChild[nIndex]->m_nLevel = m_nLevel + 1;

	vCenter = m_v3Center + vector3(nNewSize, nNewSize, -nNewSize);
	nIndex++;
	m_pChild[nIndex] = new MyOctant(vCenter, nNewSize);
	m_pChild[nIndex]->m_pParent = this;
	m_pChild[nIndex]->m_nLevel = m_nLevel + 1;

	vCenter = m_v3Center + vector3(-nNewSize, nNewSize, -nNewSize);
	nIndex++;
	m_pChild[nIndex] = new MyOctant(vCenter, nNewSize);
	m_pChild[nIndex]->m_pParent = this;
	m_pChild[nIndex]->m_nLevel = m_nLevel + 1;

	vCenter = m_v3Center + vector3(-nNewSize, -nNewSize, -nNewSize);
	nIndex++;
	m_pChild[nIndex] = new MyOctant(vCenter, nNewSize);
	m_pChild[nIndex]->m_pParent = this;
	m_pChild[nIndex]->m_nLevel = m_nLevel + 1;

	vCenter = m_v3Center + vector3(nNewSize, -nNewSize, -nNewSize);
	nIndex++;
	m_pChild[nIndex] = new MyOctant(vCenter, nNewSize);
	m_pChild[nIndex]->m_pParent = this;
	m_pChild[nIndex]->m_nLevel = m_nLevel + 1;
}

MyOctant* MyOctant::GetChild(int nIndex)
{
	if (nIndex < 0 || nIndex >7)
		return nullptr;
	return m_pChild[nIndex];
}

void MyOctant::KillBranch()
{
	for (int nChild = 0; nChild < m_nChildren; nChild++)
	{
		m_pChild[nChild]->KillBranch();
	}
	for (int nChild = 0; nChild < m_nChildren; nChild++)
	{
		if (m_pChild[nChild] != nullptr)
		{
			if (m_pChild[nChild]->m_pBObj != nullptr)
			{
				delete m_pChild[nChild]->m_pBObj;
				m_pChild[nChild]->m_pBObj = nullptr;
			}
			delete m_pChild[nChild];
			m_pChild[nChild] = nullptr;
		}
	}
}

void MyOctant::CheckSubdivision(){
	//check to see how many things are in the octant
	// if more than three and the level is less than 3, subdivide
	if (objectList.size() > 3 && m_nLevel < 3){
		Subdivide();
		for (int i = 0; i < m_nChildren; i++){
			//add objects from parents
			m_pChild[i]->AddToObjects();
		}
	}

}

void MyOctant::AddToObjects(){
	if (m_pParent != nullptr)
	{
		//add to child nodes if the object is in the node
		for (int i = 0; i < m_pParent->GetObjects().size(); i++)
		{
			if (m_pBObj->IsColliding(m_pParent->GetObjects()[i]))
			{
				objectList.push_back(m_pParent->GetObjects()[i]);
			}
		}

		//check to see if subdividing is needed
		CheckSubdivision();
	}
}

std::vector<MyBOClass*> MyOctant::GetObjects() { return objectList; }

void MyOctant::PushToRootObjectList(MyBOClass* objectIn)
{
	if (m_pParent == nullptr)
	{
		objectList.push_back(objectIn);
	}
}

void MyOctant::CheckCollision()
{
	// if the node has children go check collision within the children
	if (m_nChildren > 0)
	{
		for (int i = 0; i < m_nChildren; i++)
		{
			m_pChild[i]->CheckCollision();
		}
	}
	else if (objectList.size() > 0)
	{
		collidedIndex.clear(); //clear the collided index
		for (int i = 0; i < objectList.size() -1; i++)
		{
			for (int j = i + 1; j < objectList.size(); j++)
			{
				if (objectList[i]->IsColliding(objectList[j]))
				{
					bool iExists = false;
					bool jExists = false;
					
					for (int k = 0; k < collidedIndex.size(); k++)
					{
						if (i == collidedIndex[k])
							iExists = true;
					
						if (j == collidedIndex[k])
							jExists = true;
					}
					//only push it to the idex if it didn't already exist
					if (!iExists)
						collidedIndex.push_back(i);
					if (!jExists)
						collidedIndex.push_back(j);
				}
			}
		}
	}
}

void MyOctant::DrawObjects()
{
	//draws the object in red if they are in the colliding list
	if (m_nChildren > 0)
	{
		for (int i = 0; i < m_nChildren; i++)
		{
			m_pChild[i]->DrawObjects();
		}
	}
	else
	{
		if (objectList.size() > 0)
		{
			for (int i = 0; i < objectList.size(); i++)
			{
				objectList[i]->DisplayOriented(REWHITE);
			}
			for (int i = 0; i < collidedIndex.size(); i++)
			{
				objectList[collidedIndex[i]]->DisplayOriented(RERED);
			}
		}
	}
}

void MyOctant::AddObjectList(std::vector<MyBOClass*> list)
{
	// if this is the root, add the list to the object list
	if (m_pParent == nullptr)
	{
		for (int i = 0; i < list.size(); i++)
		{
			objectList.push_back(list[i]);
		}
	}
}