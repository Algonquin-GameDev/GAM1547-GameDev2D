#include "Transformable.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
	Transformable::Transformable() :
		m_Transform(Matrix::Identity()),
		m_Rotation(),
		m_Position(0.0f, 0.0f),
		m_Scale(1.0f, 1.0f),
		m_Parent(nullptr),
		m_IsTransformMatrixDirty(false)
	{

	}

	Transformable::Transformable(const Transformable& aTransformable) :
		m_Transform(aTransformable.m_Transform),
		m_Rotation(aTransformable.m_Rotation),
		m_Position(aTransformable.m_Position),
		m_Scale(aTransformable.m_Scale),
		m_Parent(aTransformable.m_Parent),
		m_IsTransformMatrixDirty(aTransformable.m_IsTransformMatrixDirty)
	{

	}

	Transformable::~Transformable()
	{

	}

	Matrix Transformable::GetTransformMatrix()
	{
		if (m_IsTransformMatrixDirty == true)
		{
			Matrix translationMatrix = Matrix::MakeTranslation(m_Position);
			Matrix rotationMatrix = Matrix::MakeRotation(m_Rotation);
			Matrix scaleMatrix = Matrix::MakeScale(m_Scale);

			m_Transform = translationMatrix * rotationMatrix * scaleMatrix;
			m_IsTransformMatrixDirty = false;
		}

		if (m_Parent != nullptr)
		{
			return m_Parent->GetTransformMatrix() * m_Transform;
		}

		return m_Transform;
	}

	Vector2 Transformable::GetWorldPosition()
	{
		return GetTransformMatrix().GetTranslation();
	}

	Vector2 Transformable::GetPosition()
	{
		return m_Position;
	}

	void Transformable::SetPosition(Vector2 aPosition)
	{
		m_Position = aPosition;
		TransformMatrixIsDirty();
	}

	void Transformable::SetPosition(float aX, float aY)
	{
		m_Position.x = aX;
		m_Position.y = aY;
		TransformMatrixIsDirty();
	}

	void Transformable::SetPositionX(float aX)
	{
		m_Position.x = aX;
		TransformMatrixIsDirty();
	}

	void Transformable::SetPositionY(float aY)
	{
		m_Position.y = aY;
		TransformMatrixIsDirty();
	}

	void Transformable::Translate(Vector2 aOffset)
	{
		m_Position += aOffset;
		TransformMatrixIsDirty();
	}

	float Transformable::GetWorldDegrees()
	{
		return GetTransformMatrix().GetDegrees();
	}

	float Transformable::GetWorldRadians()
	{
		return GetTransformMatrix().GetRadians();
	}

	float Transformable::GetDegrees()
	{
		return m_Rotation.GetDegrees();
	}

	float Transformable::GetRadians()
	{
		return m_Rotation.GetRadians();
	}

	void Transformable::SetDegrees(float aDegrees)
	{
		m_Rotation.SetDegrees(aDegrees);
		TransformMatrixIsDirty();
	}

	void Transformable::SetRadians(float aRadians)
	{
		m_Rotation.SetRadians(aRadians);
		TransformMatrixIsDirty();
	}

	void Transformable::Rotate(Rotation& aRotation)
	{
		m_Rotation.SetRadians(m_Rotation.GetRadians() + aRotation.GetRadians());
		TransformMatrixIsDirty();
	}

	void Transformable::RotateDegrees(float aDegrees)
	{
		Rotate(Rotation::Degrees(aDegrees));
	}

	void Transformable::RotateRadians(float aRadians)
	{
		Rotate(Rotation::Radians(aRadians));
	}

	Vector2 Transformable::GetDirection()
	{
		return m_Rotation.GetDirection();
	}

	void Transformable::SetDirection(Vector2 aDirection)
	{
		m_Rotation.SetDirection(aDirection);
		TransformMatrixIsDirty();
	}

	Vector2 Transformable::GetWorldScale()
	{
		return GetTransformMatrix().GetScale();
	}

	Vector2 Transformable::GetScale()
	{
		return m_Scale;
	}

	void Transformable::SetScale(Vector2 aScale)
	{
		m_Scale = aScale;
		TransformMatrixIsDirty();
	}

	void Transformable::SetScale(float aSacleX, float aScaleY)
	{
		m_Scale.x = aSacleX;
		m_Scale.y = aScaleY;
		TransformMatrixIsDirty();
	}

	void Transformable::SetScaleX(float aSacleX)
	{
		m_Scale.x = aSacleX;
		TransformMatrixIsDirty();
	}

	void Transformable::SetScaleY(float aSacleY)
	{
		m_Scale.y = aSacleY;
		TransformMatrixIsDirty();
	}

	bool Transformable::IsTransformationMatrixDirty()
	{
		return m_IsTransformMatrixDirty;
	}

	void Transformable::AttachTo(Transformable* parent)
	{
		m_Parent = parent;
	}

	Transformable* Transformable::GetParent()
	{
		return m_Parent;
	}

	void Transformable::Log()
	{
		Log::Message(Log::Verbosity_Debug, "[Transformable] Position(%f, %f), Rotation(%f), Scale(%f, %f)", m_Position.x, m_Position.y, m_Rotation.GetRadians(), m_Scale.x, m_Scale.y);
	}

	void Transformable::TransformMatrixIsDirty()
	{
		m_IsTransformMatrixDirty = true;
	}
}