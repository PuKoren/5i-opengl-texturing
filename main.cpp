#include <gl\glew.h>
#include <gl\freeglut.h>
#include "include/config.h"

#include "EsgiGL/EsgiShader.h"
#include "ESGIGL/common/EsgiTimer.h"
#include "EsgiGL/common/EsgiTGA.h"
#include "EsgiGL/common/matrix.h"

EsgiShader shader;

mat4 projectionMatrix;
mat4 viewMatrix;

struct EsgiSprite
{
	vec2	m_Position;
	vec2	m_Dimension;
	vec4	m_Color;
	vec2	m_Repeat;
    GLuint  m_TextureId;
	float	m_Orientation;
	float	m_RotationSpeed;

	static unsigned short indices[];

	void Process(float elapsedTime)
	{
		m_Orientation += m_RotationSpeed * elapsedTime;
		m_Orientation = fmodf(m_Orientation, 360.f);
	}

	void Render(GLuint programObject)
	{
		struct vertex_attribute
		{
			vec2 position;
			vec2 texcoord;
		} quad[4];
      
		// le sprite dans le repere de l'objet
		quad[0].position = m_Dimension*0.5f;
        quad[0].texcoord = vec2(1.f, 1.f);
		quad[1].position = vec2(m_Dimension.x*0.5f, -m_Dimension.y*0.5f);
		quad[1].texcoord = vec2(1.f, 0.f);
        quad[2].position = m_Dimension*-0.5f;
        quad[2].texcoord = vec2(0.f, 0.f);
		quad[3].position = vec2(-m_Dimension.x*0.5f, m_Dimension.y*0.5f);
		quad[3].texcoord = vec2(0.f, 1.f);
        
        // tourne autour de l'axe Z du monde
		mat4 world = esgiRotateZ(m_Orientation);
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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
        glEnableVertexAttribArray(texcoord_attribute);
		glDisableVertexAttribArray(position_attribute);
        
        
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
	}
};

unsigned short EsgiSprite::indices[] = { 0, 1, 2, 2, 3, 0 };

EsgiSprite sprite;

// ---

// --- Fonctions -------------------------------------------------------------
double m_PreviousFrameTime = 0;
double m_ElapsedTime = 0;
void Update()
{
    m_ElapsedTime = EsgiTimer::GetElapsedTimeSince(m_PreviousFrameTime);
	m_PreviousFrameTime = EsgiTimer::GetTimerValue();
	sprite.Process(m_ElapsedTime);
    glutSwapBuffers();
}

void Draw()
{
	// efface le color buffer
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// ---
	GLuint programObject = shader.GetProgram();
	glUseProgram(programObject);
	// alternativement
	// shader.Bind();	
	// ---

	projectionMatrix = esgiOrtho(0.f, 800.f, 0.f, 600.f, 0.f, 1.f);	
	GLint projectionUniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, 0, &projectionMatrix.I.x);	

	sprite.Render(programObject);
    
	// alternativement
	// shader.Unbind();
}


// ---

bool Setup()
{
	if(shader.LoadVertexShader("../resources/simpleSprite.vert") && shader.LoadFragmentShader("../resources/simpleSprite.frag")){
	    shader.Create();
    }else{
        return false;
    }

	// camera a l'origine
	viewMatrix.Identity();

    EsgiTexture *logo = esgiReadTGAFile("../resources/logo-esgi.tga");
	if (logo == NULL) {
		return false;
	}
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glGenTextures(1, &sprite.m_TextureId);
    glBindTexture(GL_TEXTURE_2D, sprite.m_TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, logo->internalFormat, logo->width, logo->height
                 , 0, logo->format, logo->datatype, logo->texels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // la texture restera constamment en vram, on peut la supprimer entierement
    delete[] logo->texels;
    delete logo;
    
	sprite.m_Position = vec2(400.f, 300.f);
	sprite.m_Dimension = vec2(256.f, 256.f);
	sprite.m_RotationSpeed = 360.0f;
	sprite.m_Orientation = 0.f;
	sprite.m_Color.set(1.f, 0.5f, 0.5f, 1.f);

	return true;
}

void Clean()
{
    if (sprite.m_TextureId)
    {
        glDeleteTextures(1, &sprite.m_TextureId);
    }    
	shader.Destroy();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Texturing - OpenGL");
    glewInit();

    Setup();

    glutIdleFunc(Update);
	glutDisplayFunc(Draw);
    glutMainLoop();

    Clean();
    glutExit();
    return EXIT_SUCCESS;
}