#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"
#include <string>

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	: MeshComponent(owner, true)
	, mSkeleton(nullptr)
{
	printf("    Create : SkeletalMeshComponent\n");
}

SkeletalMeshComponent::~SkeletalMeshComponent()
{
	printf("Remove : SkeletalMeshComponent\n    ");
}

void SkeletalMeshComponent::Draw(Shader* shader)                         // 描画
{
	if (mMesh)
	{
		// Set the world transform                                        ワールド変換をセット
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		// Set the matrix palette                                         行列パレットをセット    
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0],
			MAX_SKELETON_BONES);
		// Set specular power                                             スペキュラー強度をセット
		shader->SetFloatUniform("uSpecPower", 100);
		// Set the active texture                                         テクスチャをセット 
		//Texture* t = mMesh->GetTexture(mTextureIndex);
		//if (t)
		//{
		//	t->SetActive();
		//}

		SetTextureToShader(shader);

		// Set the mesh's vertex array as active                          メッシュの頂点配列をアクティブに
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw                                                           描画
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;

		// アニメーションがループアニメーションなら巻き戻し処理
		if (mAnimation->IsLoopAnimation())
		{
			// Wrap around anim time if past duration                         アニメを巻き戻して再生
			while (mAnimTime > mAnimation->GetDuration())
			{
				mAnimTime -= mAnimation->GetDuration();
			}
		}
		// ループしないアニメで再生時間超えたら最終時間までとする
		else if (mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime = mAnimation->GetDuration();
		}
		// Recompute matrix palette                                      行列パレットの再計算
		ComputeMatrixPalette();
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate)  // アニメーションの再生
{
	mAnimation = anim;
	mAnimTime = 0.0f;
	mAnimPlayRate = playRate;

	if (!mAnimation) { return 0.0f; }

	ComputeMatrixPalette();

	return mAnimation->GetDuration();
}

// 現在アニメーション再生中か？ true : 再生中 / false : 再生終了
bool SkeletalMeshComponent::IsPlaying()
{
	if (!mAnimation->IsLoopAnimation())
	{
		if (mAnimTime >= mAnimation->GetDuration())
		{
			return false;
		}
	}
	return true;
}

// ボーン名の文字列からボーンインデックスを検索
int SkeletalMeshComponent::GetBoneIndexFromName(const char* boneName) const
{
	std::string name;
	name = boneName;
	return mSkeleton->GetBoneIndexFromName(name);
}

// ボーンインデックス値からそのボーンの最終ワールド行列を返す
void SkeletalMeshComponent::GetMatrixFromBoneIndex(Matrix4& boneWorldMatrix, int boneIndex) const
{
	boneWorldMatrix = mPalette.mEntry[boneIndex] * mOwner->GetWorldTransform();
}

void SkeletalMeshComponent::ComputeMatrixPalette()                              // 行列パレットの計算
{
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvBindPoses();   // グローバル逆バインド行列配列の取得
	std::vector<Matrix4> currentPoses;                                         // 現在のポーズ行列
	mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);       // アニメ時刻時点のグローバルポーズの取得

	// Setup the palette for each bone                                        それぞれのボーンの行列パレットのセット
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
		// Global inverse bind pose matrix times current pose matrix 　　　　　行列パレット[i] = グローバル逆バインド行列[i]　×　現在のポーズ行列[i]  (iはボーンID)         
		mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}
