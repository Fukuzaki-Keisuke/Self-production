#pragma once
#include "Actor.h"
class BGBlock : public Actor
{
public:
	BGBlock();
	~BGBlock();
	void SetMesh(class Mesh* mesh);

private:
	Vector3 mPos;
	class MeshComponent* mMeshComp;
	class BoxCollider* mBox;
};