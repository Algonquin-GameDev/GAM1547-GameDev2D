#include "Transformable.h"
#include "../Debug/Log.h"


namespace GameDev2D
{
    Transformable::Transformable() :
        m_Transform(Matrix::Identity()),
        m_Rotation(),
        m_Position(0.0f, 0.0f),
        m_Scale(1.0f, 1.0f),
        m_IsTransformMatrixDirty(false)
	{

    }

    Transformable::Transformable(const Transformable& aTransformable) :
        m_Transform(aTransformable.m_Transform),
        m_Rotation(aTransformable.m_Rotation),
        m_Position(aTransformable.m_Position),
        m_Scale(aTransformable.m_Scale),
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

        return m_Transform;
    }

    Vector2 Transformable::GetPosition()
    {
        return m_Position;
    }

    void Transformable::SetPosition(Vector2 aPosition)
    {
        m_Position = aPosition;
        m_IsTransformMatrixDirty = true;
    }

	void Transformable::SetPosition(float aX, float aY)
	{
		m_Position.x = aX;
		m_Position.y = aY;
		m_IsTransformMatrixDirty = true;
	}

	void Transformable::SetPositionX(float aX)
	{
		m_Position.x = aX;
		m_IsTransformMatrixDirty = true;
	}

	void Transformable::SetPositionY(float aY)
	{
		m_Position.y = aY;
		m_IsTransformMatrixDirty = true;
	}

    void Transformable::Translate(Vector2 aOffset)
    {
        m_Position += aOffset;
        m_IsTransformMatrixDirty = true;
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
        m_IsTransformMatrixDirty = true;
    }

    void Transformable::SetRadians(float aRadians)
    {
        m_Rotation.SetRadians(aRadians);
        m_IsTransformMatrixDirty = true;
    }

    void Transformable::Rotate(Rotation& aRotation)
    {
        m_Rotation.SetRadians(m_Rotation.GetRadians() + aRotation.GetRadians());
        m_IsTransformMatrixDirty = true;
    }

    Vector2 Transformable::GetDirection()
    {
        return m_Rotation.GetDirection();
    }

    void Transformable::SetDirection(Vector2 aDirection)
    {
        m_Rotation.SetDirection(aDirection);
        m_IsTransformMatrixDirty = true;
    }

    Vector2 Transformable::GetScale()
    {
        return m_Scale;
    }

    void Transformable::SetScale(Vector2 aScale)
    {
        m_Scale = aScale;
        m_IsTransformMatrixDirty = true;
    }

	void Transformable::SetScale(float aSacleX, float aScaleY)
	{
		m_Scale.x = aSacleX;		
		m_Scale.y = aScaleY;
		m_IsTransformMatrixDirty = true;
	}

	void Transformable::SetScaleX(float aSacleX)
	{
		m_Scale.x = aSacleX;
		m_IsTransformMatrixDirty = true;
	}

	void Transformable::SetScaleY(float aSacleY)
	{
		m_Scale.y = aSacleY;
		m_IsTransformMatrixDirty = true;
	}

    void Transformable::Log()
    {
        Log::Message(Log::Verbosity_Debug, "[Transformable] Position(%f, %f), Rotation(%f), Scale(%f, %f)", m_Position.x, m_Position.y, m_Rotation.GetRadians(), m_Scale.x, m_Scale.y);
    }
}