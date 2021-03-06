#ifndef _ESGI_SPRITE
#define _ESGI_SPRITE

#include <GL/glew.h>
#include "common/matrix.h"

struct EsgiSprite
{
	vec2	m_Position;
	vec2	m_Dimension;
	vec4	m_Color;
	vec2	m_Repeat;
    mat4    viewMatrix;
    GLuint  m_TextureId;
	float	m_Orientation;
    float   m_OrientationX;
	float	m_RotationSpeed;

	unsigned short* indices;

	void Process(float elapsedTime)
	{
        /*
		m_Orientation += m_RotationSpeed * elapsedTime;
		m_Orientation = fmodf(m_Orientation, 360.f);
        */
	}

    void Render(GLuint programObject)
	{
		struct vertex_attribute
		{
			vec2 position;
			vec2 texcoord;
        } quad[6];
      
		// le sprite dans le repere de l'objet
        //top left
        quad[0].position = m_Dimension * -0.5f;
        quad[0].texcoord = vec2(0.f, 0.f);
        //top middle
        quad[1].position = vec2(0.f, m_Dimension.y * -0.5f);
        quad[1].texcoord = vec2(0.5f, 0.f);
        //top right
        quad[2].position = vec2(m_Dimension.x * 0.5f, m_Dimension.y * -0.5f);
        quad[2].texcoord = vec2(1.f, 0.f);
        //bottom right
        quad[3].position = m_Dimension*0.5f;
        quad[3].texcoord = vec2(1.f, 1.f);
        //bottom middle
        quad[4].position = vec2(0.f, m_Dimension.y*0.5f);
        quad[4].texcoord = vec2(0.5f, 1.f);
        //bottom left
        quad[5].position = vec2(m_Dimension.x*-0.5f, m_Dimension.y*0.5f);
        quad[5].texcoord = vec2(0.f, 1.f);
        
        // tourne autour de l'axe Z du monde
        mat4 world = esgiRotateZ(m_Orientation);
        world = esgiMultiplyMatrix(world, esgiRotateX(m_OrientationX));
		world.T.set(m_Position.x, m_Position.y, 0, 1);
        
        // texture
        if (m_TextureId)
        {
            glEnable(GL_TEXTURE_2D);
            // glActiveTexture() n'est pas necessaire si l'on n'utilise qu'une seule unite de texture
            // Sert a indiquer a quelle unite de texture s'applique l'appel a glBindTexture()
            glActiveTexture(GL_TEXTURE0);        
            glBindTexture(GL_TEXTURE_2D, m_TextureId);
            
            GLuint samplerUniform = glGetUniformLocation(programObject, "u_SpriteTexture");
            glUniform1i(samplerUniform, 0);
        }
		GLint modelviewUniform = glGetUniformLocation(programObject, "u_ModelViewMatrix");
		mat4 modelviewMatrix = esgiMultiplyMatrix(viewMatrix, world);
		glUniformMatrix4fv(modelviewUniform, 1, 0, &modelviewMatrix.I.x);

		GLuint diffuseUniform = glGetUniformLocation(programObject, "u_DiffuseColor");
		glUniform3fv(diffuseUniform, 1, &m_Color.x);

		GLint position_attribute = glGetAttribLocation(programObject, "a_Position");		
		glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), quad);

		GLint texcoord_attribute = glGetAttribLocation(programObject, "a_TexCoord");		
		glVertexAttribPointer(texcoord_attribute, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), &quad[0].texcoord);		
        
        glEnableVertexAttribArray(position_attribute);
        glEnableVertexAttribArray(texcoord_attribute);

        glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_SHORT, indices);

        glDisableVertexAttribArray(texcoord_attribute);
		glDisableVertexAttribArray(position_attribute);

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
	}
};

#endif
