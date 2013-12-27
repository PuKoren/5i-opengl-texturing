#include "TP1.h"

TP1::TP1(){
    sprite.indices = new unsigned short[6];
    sprite.indices[0] = 0;
    sprite.indices[1] = 1;
    sprite.indices[2] = 2;
    sprite.indices[3] = 2;
    sprite.indices[4] = 3;
    sprite.indices[5] = 0;

    m_ElapsedTime = 0;
    m_PreviousFrameTime = 0;
}

TP1::~TP1(){
    if(sprite.m_TextureId)
    {
        glDeleteTextures(1, &sprite.m_TextureId);
    }    
	shader.Destroy();
    delete[] sprite.indices;
}

bool TP1::Init(){
    if(shader.LoadVertexShader("../resources/simpleSprite.vert") && shader.LoadFragmentShader("../resources/simpleSprite.frag")){
	    shader.Create();
    }else{
        return false;
    }

	// camera a l'origine
	sprite.viewMatrix.Identity();

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
    
    sprite.m_Position = vec2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
	sprite.m_Dimension = vec2(256.f, 256.f);
	sprite.m_RotationSpeed = 180.f;
	sprite.m_Orientation = 0.f;
	sprite.m_Color.set(1.f, 0.5f, 0.5f, 1.f);

	return true;
}

void TP1::Update(){
    m_ElapsedTime = EsgiTimer::GetElapsedTimeSince(m_PreviousFrameTime);
	m_PreviousFrameTime = EsgiTimer::GetTimerValue();
	sprite.Process((float)m_ElapsedTime);
}

void TP1::MouseMove(int x, int y){
    Vector2<float> mousePos((float)x, (float)y);
    Vector2<float> spritePos((float)sprite.m_Position.x, (float)sprite.m_Position.y);

    //difference between 2 vectors to compute normalized direction
    mousePos -= spritePos;
    mousePos.Normalize();

    //dot product for acos (returns angle in radians)
    double dotProduct = mousePos.Dot(Vector2<int>(0, -1));
    double angle = acos(dotProduct);

    //if x is in the right part of the screen, we compute angle in a different way (X axis reverted)
    if(x >= WINDOW_WIDTH/2){
        angle = ESGI_PI + (ESGI_PI-angle);
    }
    //setting new orientation
    sprite.m_Orientation = (float)(180.f * angle) / ESGI_PI;
}

void TP1::Draw(){
    // efface le color buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	GLuint programObject = shader.GetProgram();
	glUseProgram(programObject);

    projectionMatrix = esgiOrtho(0.f, WINDOW_WIDTH, 0.f, WINDOW_HEIGHT, 0.f, 1.f);	
	GLint projectionUniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	glUniformMatrix4fv(projectionUniform, 1, 0, &projectionMatrix.I.x);	

	sprite.Render(programObject);
    
    glutSwapBuffers();
    glutPostRedisplay();
}