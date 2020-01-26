#ifndef __GameDev2D__Drawable__
#define __GameDev2D__Drawable__

#include "Transformable.h"
#include "../Graphics/Color.h"
#include "../Graphics/GraphicTypes.h"
#include "../Graphics/Shader.h"


namespace GameDev2D
{
    class Drawable : public Transformable
    {
    public:
        Drawable();
        virtual ~Drawable();

        //All classes that inherit from Drawable must implement the Draw() method to 
        //ensure consistency across all Drawable objects (hence why its pure virtual)
        virtual void Draw() = 0;

        //All classes that inherit from Drawable must implement the GetWidth() and GetHeight() 
        //methods to ensure consistency across all Drawable objects (hence why its pure virtual)
        virtual float GetWidth() = 0;
        virtual float GetHeight() = 0;

        //Returns the Shader associated with the Drawable object
        Shader* GetShader();

        //Sets the Shader associated with the Drawable object
        virtual void SetShader(Shader* shader);

        //Returns the Color associated with the Drawable object
        Color GetColor();

        //Sets the Color associated with the Drawable object
        virtual void SetColor(Color color);

		//Returns the Alpha associated with the Drawable object
		float GetAlpha();

		//Sets the Alpha associated with the Drawable object between 0.0-1.0
		virtual void SetAlpha(float alpha);

        //Returns the anchor point associated with the Drawable object
        Vector2 GetAnchor();

        //Sets the anchor point associated with the Drawable object
        void SetAnchor(Vector2 anchor);

		//Sets the anchor point associated with the Drawable object
		void SetAnchor(float anchorX, float anchorY);

		//Sets the anchor point along the y-axis associated with the Drawable object
		void SetAnchorX(float anchorX);

		//Sets the anchor point along the y-axis associated with the Drawable object
		void SetAnchorY(float anchorY);

		//Sets the blending mode of the Drawable object
		void SetBlendingMode(BlendingFactor source, BlendingFactor destination);
		void SetBlendingMode(BlendingMode blendingMode);

		//Returns the blending mode of the Drawable object
		BlendingMode GetBlendingMode();

    protected:
        //Member variables
        Shader* m_Shader;
        Color m_Color;
        Vector2 m_Anchor;
		BlendingMode m_BlendingMode;
    };
}

#endif