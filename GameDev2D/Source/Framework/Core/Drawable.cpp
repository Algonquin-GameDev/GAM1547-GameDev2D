#include "Drawable.h"


namespace GameDev2D
{
    Drawable::Drawable() : Transformable(),
        m_Shader(nullptr),
        m_Color(Color::WhiteColor()),
        m_Anchor(0.0f, 0.0f),
		m_BlendingMode(BlendingMode()),
		m_Edges{},
		m_EdgesCalculationDirty(true)
    {

    }

    Drawable::~Drawable()
    {

    }

    Shader* Drawable::GetShader()
    {
        return m_Shader;
    }

    void Drawable::SetShader(Shader* aShader)
    {
        m_Shader = aShader;
    }

    Color Drawable::GetColor()
    {
        return m_Color;
    }

    void Drawable::SetColor(Color aColor)
    {
        m_Color = aColor;
    }

	float Drawable::GetAlpha()
	{
		return m_Color.a;
	}

	void Drawable::SetAlpha(float aAlpha)
	{
		Color color = m_Color;
		color.a = aAlpha;
		SetColor(color);
	}

    Vector2 Drawable::GetAnchor()
    {
        return m_Anchor;
    }

    void Drawable::SetAnchor(Vector2 aAnchor)
    {
        m_Anchor = aAnchor;
    }

	void Drawable::SetAnchor(float aAnchorX, float aAnchorY)
	{
		m_Anchor.x = aAnchorX;
		m_Anchor.y = aAnchorY;
	}

	void Drawable::SetAnchorX(float aAnchorX)
	{
		m_Anchor.x = aAnchorX;
	}

	void Drawable::SetAnchorY(float aAnchorY)
	{
		m_Anchor.y = aAnchorY;
	}

	void Drawable::SetBlendingMode(BlendingFactor aSource, BlendingFactor aDestination)
	{
		m_BlendingMode.source = aSource;
		m_BlendingMode.destination = aDestination;
	}

	void Drawable::SetBlendingMode(BlendingMode aBlendingMode)
	{
		m_BlendingMode = aBlendingMode;
	}

	BlendingMode Drawable::GetBlendingMode()
	{
		return m_BlendingMode;
	}

	float Drawable::GetLeftEdge()
	{
		if (m_EdgesCalculationDirty == true || m_Parent != nullptr)
		{
			CalculateEdges();
		}

		return m_Edges[LeftEdge];
	}

	float Drawable::GetRightEdge()
	{
		if (m_EdgesCalculationDirty == true || m_Parent != nullptr)
		{
			CalculateEdges();
		}

		return m_Edges[RightEdge];
	}

	float Drawable::GetTopEdge()
	{
		if (m_EdgesCalculationDirty == true || m_Parent != nullptr)
		{
			CalculateEdges();
		}

		return m_Edges[TopEdge];
	}

	float Drawable::GetBottomEdge()
	{
		if (m_EdgesCalculationDirty == true || m_Parent != nullptr)
		{
			CalculateEdges();
		}

		return m_Edges[BottomEdge];
	}

	void Drawable::TransformMatrixIsDirty()
	{
		m_EdgesCalculationDirty = true;
		Transformable::TransformMatrixIsDirty();
	}

	void Drawable::CalculateEdges()
	{
		Vector2 position = GetTransformMatrix().GetTranslation();
		float radians = GetTransformMatrix().GetRadians();
		float w = GetWidth();
		float h = GetHeight();
		float c = cosf(radians);
		float s = sinf(radians);
		float cw = c * w;
		float ch = c * h;
		float sw = s * w;
		float sh = s * h;
		float ex = (fabsf(sh) + fabsf(cw)) * 0.5f;
		float ey = (fabsf(sw) + fabsf(ch)) * 0.5f;
		float ax = GetAnchor().x;
		float ay = GetAnchor().y;

		//Calculate the edge positions
		m_Edges[LeftEdge] = position.x + sh * (ay - 0.5) - cw * (ax - 0.5) - ex;
		m_Edges[RightEdge] = position.x + sh * (ay - 0.5) - cw * (ax - 0.5) + ex;
		m_Edges[TopEdge] = position.y - sw * (ax - 0.5) - ch * (ay - 0.5) + ey;
		m_Edges[BottomEdge] = position.y - sw * (ax - 0.5) - ch * (ay - 0.5) - ey;
		m_EdgesCalculationDirty = false;
	}
}