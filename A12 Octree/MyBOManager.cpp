#include "MyBOManager.h"
//  MyBOManager
MyBOManager* MyBOManager::m_pInstance = nullptr;
void MyBOManager::Init(void)
{
	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_nObjectCount = 0;
}
void MyBOManager::Release(void)
{
	for (uint nObject = 0; nObject < m_nObjectCount; nObject++)
	{
		if (m_lObject[nObject] != nullptr)
		{
			delete m_lObject[nObject];
			m_lObject[nObject] = nullptr;
		}
		m_llCollidingIndices[nObject].clear();
	}
	m_lObject.clear();
	m_llCollidingIndices.clear();
	if (m_pRoot != nullptr)
	{
		m_pRoot->KillBranch();
		delete m_pRoot;
		m_pRoot = nullptr;
	}

}
MyBOManager* MyBOManager::GetInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new MyBOManager();
	}
	return m_pInstance;
}
void MyBOManager::ReleaseInstance()
{
	if(m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//The big 3
MyBOManager::MyBOManager(){ Init(); }
MyBOManager::MyBOManager(MyBOManager const& other){ }
MyBOManager& MyBOManager::operator=(MyBOManager const& other) { return *this; }
MyBOManager::~MyBOManager(){Release();};
//Accessors
MyBOClass* MyBOManager::GetBoundingObject(String a_sIndex)
{
	int nIndex = GetIndex(a_sIndex);
	return GetBoundingObject(nIndex);
}
MyBOClass* MyBOManager::GetBoundingObject(uint a_nIndex)
{
	if (a_nIndex < m_nObjectCount)
		return m_lObject[a_nIndex];

	return nullptr;
}
//--- Non Standard Singleton Methods
void MyBOManager::AddObject(String a_sName)
{
	std::vector<vector3> lVertex = m_pMeshMngr->GetVertexList(a_sName);
	AddObject(lVertex, a_sName);
	SetModelMatrix(m_pMeshMngr->GetModelMatrix(a_sName), a_sName);
}
void MyBOManager::AddObject(std::vector<vector3> a_lVertex, String a_sName)
{
	MyBOClass* pObject = new MyBOClass(a_lVertex);
	if (pObject != nullptr)
	{
		m_lObject.push_back(pObject);//Add the Object
		m_mapIndex[a_sName] = m_nObjectCount; //Add entry to the dictionary
	}
	m_nObjectCount = m_lObject.size();
	std::vector<int> lVector;
	m_llCollidingIndices.push_back(lVector);
}
void MyBOManager::SetModelMatrix(matrix4 a_mModelMatrix, String a_sIndex)
{
	//find the object
	int nIndex = this->GetIndex(a_sIndex);
	if (nIndex < 0) //if not found return
		return;

	m_lObject[nIndex]->SetModelMatrix(a_mModelMatrix);//set the matrix for the indexed Object
}
void MyBOManager::DisplaySphere(String a_sName, vector3 a_v3Color)
{
	int nIndex = GetIndex(a_sName);
	DisplaySphere(nIndex, a_v3Color);
}
void MyBOManager::DisplaySphere(int a_nIndex, vector3 a_v3Color)
{
	//If the index is larger than the number of objects stored return with no changes
	if (a_nIndex >= static_cast<int>(m_nObjectCount))
		return;

	vector3 v3Color = a_v3Color;
	//if The index exists
	if (a_nIndex >= 0)
	{
		//Default will render in white for non colliding and red for colliding
		if (v3Color == REDEFAULT)
		{
			v3Color = REWHITE;
			if (m_llCollidingIndices[a_nIndex].size() > 0)
				v3Color = RERED;
		}
		m_lObject[a_nIndex]->DisplaySphere(v3Color);
	}
	else //all objects sent
	{
		for (uint nObject = 0; nObject < m_nObjectCount; nObject++)
		{
			//Default will render in white for non colliding and red for colliding
			if (v3Color == REDEFAULT)
			{
				v3Color = REWHITE;
				if (m_llCollidingIndices[nObject].size() > 0)
					v3Color = RERED;
			}
			m_lObject[nObject] ->DisplaySphere(v3Color);
		}
	}
}
void MyBOManager::DisplayOriented(String a_sName, vector3 a_v3Color)
{
	int nIndex = GetIndex(a_sName);
	DisplayOriented(nIndex, a_v3Color);
}
void MyBOManager::DisplayOriented(int a_nIndex, vector3 a_v3Color)
{
	//If the index is larger than the number of objects stored return with no changes
	if (a_nIndex >= static_cast<int>(m_nObjectCount))
		return;

	vector3 v3Color = a_v3Color;
	//if The index exists
	if (a_nIndex >= 0)
	{
		//Default will render in white for non colliding and red for colliding
		if (v3Color == REDEFAULT)
		{
			v3Color = REWHITE;
			if (m_llCollidingIndices[a_nIndex].size() > 0)
				v3Color = RERED;
		}
		m_lObject[a_nIndex]->DisplayOriented(v3Color);
	}
	else
	{
		for (uint nObject = 0; nObject < m_nObjectCount; nObject++)
		{
			//Default will render in white for non colliding and red for colliding
			if (v3Color == REDEFAULT)
			{
				v3Color = REWHITE;
				if (m_llCollidingIndices[nObject].size() > 0)
					v3Color = RERED;
			}
			m_lObject[nObject]->DisplayOriented( v3Color );
		}
	}
}
void MyBOManager::DisplayReAlligned(String a_sName, vector3 a_v3Color)
{
	int nIndex = GetIndex(a_sName);
	DisplayReAlligned(nIndex, a_v3Color);
}
void MyBOManager::DisplayReAlligned(int a_nIndex, vector3 a_v3Color)
{
	//If the index is larger than the number of objects stored return with no changes
	if (a_nIndex >= static_cast<int>(m_nObjectCount))
		return;

	vector3 v3Color = a_v3Color;
	//if The index exists
	if (a_nIndex >= 0)
	{
		//Default will render in white for non colliding and red for colliding
		if (v3Color == REDEFAULT)
		{
			v3Color = REWHITE;
			if (m_llCollidingIndices[a_nIndex].size() > 0)
				v3Color = RERED;
		}
		m_lObject[a_nIndex]->DisplayReAlligned(v3Color);
	}
	else
	{
		for (uint nObject = 0; nObject < m_nObjectCount; nObject++)
		{
			//Default will render in white for non colliding and red for colliding
			if (v3Color == REDEFAULT)
			{
				v3Color = REWHITE;
				if (m_llCollidingIndices[nObject].size() > 0)
					v3Color = RERED;
			}
			m_lObject[nObject]->DisplayReAlligned(v3Color);
		}
	}
}
void MyBOManager::Update(bool tree)
{
	for (uint nObject = 0; nObject < m_nObjectCount; nObject++)
	{
		m_llCollidingIndices[nObject].clear();
	}

	if (tree)
		CheckCollisionTree();
	else
		CheckCollisions();
}
void MyBOManager::CheckCollisions(void)
{
	for (uint nObjectA = 0; nObjectA < m_nObjectCount -1; nObjectA++)
	{
		for (uint nObjectB = nObjectA + 1; nObjectB < m_nObjectCount; nObjectB++)
		{
			if (m_lObject[nObjectA]->IsColliding(m_lObject[nObjectB]))
			{
				m_llCollidingIndices[nObjectA].push_back(nObjectB);
				m_llCollidingIndices[nObjectB].push_back(nObjectA);
			}
		}
	}
}

void MyBOManager::CheckCollisionTree()
{
	m_pRoot->CheckCollision();
}
std::vector<int> MyBOManager::GetCollidingVector(String a_sIndex)
{
	int nIndex = GetIndex(a_sIndex);
	if (nIndex < 0)
	{
		std::vector<int> empty;
		return empty;
	}
	return GetCollidingVector(static_cast<uint>(nIndex));
}
std::vector<int> MyBOManager::GetCollidingVector(uint a_nIndex)
{
	if (a_nIndex >= m_nObjectCount)
	{
		std::vector<int> empty;
		return empty;
	}
	return m_llCollidingIndices[a_nIndex];
}
int MyBOManager::GetIndex(String a_sIndex)
{
	//Find the related index
	auto var = m_mapIndex.find(a_sIndex);
	//If not found return -1
	if (var == m_mapIndex.end())
		return -1;
	return var->second;//Get the index
}

void MyBOManager::SetupOctree()
{
	//get all of the max and min gs
	std::vector<vector3> vertexList = std::vector<vector3>();
	for (uint i = 0; i < m_lObject.size(); i++)
	{
		vertexList.push_back(m_lObject[i]->GetMaxG());
		vertexList.push_back(m_lObject[i]->GetMinG());
	}

	vector3 listMax = vertexList[0];
	vector3 listMin = vertexList[0];

	for (uint i = 0; i < vertexList.size(); i++){
		if (vertexList[i].x > listMax.x)
			listMax.x = vertexList[i].x;
		else if (vertexList[i].x < listMin.x)
			listMin.x = vertexList[i].x;

		if (vertexList[i].y > listMax.y)
			listMax.y = vertexList[i].y;
		else if (vertexList[i].y < listMin.y)
			listMin.y = vertexList[i].y;

		if (vertexList[i].z > listMax.z)
			listMax.z = vertexList[i].z;
		else if (vertexList[i].z < listMin.z)
			listMin.z = vertexList[i].z;
	}

	//find center

	vector3 center = (listMax + listMin) / 2.0f;
	//find largest side of x, y, or z divide by 2

	float xLen = listMax.x - listMin.x;
	float yLen = listMax.y - listMin.y;
	float zLen = listMax.z - listMin.z;

	float maxSide = glm::max(xLen, yLen, zLen);
	m_pRoot = new MyOctant(center, maxSide / 2.0f);

	m_pRoot->AddObjectList(m_lObject);

	m_pRoot->CheckSubdivision();
}

void MyBOManager::DisplayTree()
{
	if (m_pRoot != nullptr)
	m_pRoot->DisplayBox();
}

void MyBOManager::DisplayTreeObject()
{
	m_pRoot->DrawCollidedObjects();
}
