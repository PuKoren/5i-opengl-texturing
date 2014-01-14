#include "TP2.h"

TP2::TP2(){
    sprite.indices = new unsigned short[12];
    sprite.indices[0] = 0;
    sprite.indices[1] = 1;
    sprite.indices[2] = 2;
    sprite.indices[3] = 2;
    sprite.indices[4] = 3;
    sprite.indices[5] = 0;
    sprite.indices[6] = 0;
    sprite.indices[7] = 3;
    sprite.indices[8] = 4;
    sprite.indices[9] = 4;
    sprite.indices[10] = 5;
    sprite.indices[11] = 6;

    m_ElapsedTime = 0;
    m_PreviousFrameTime = 0;
}

TP2::~TP2(){
    if(sprite.m_TextureId)
    {
        glDeleteTextures(1, &sprite.m_TextureId);
    }    
	shader.Destroy();
    delete[] sprite.indices;
}

bool TP2::Init(){
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
    
    sprite.m_Position = vec2(0, 0);
    sprite.m_Dimension = vec2(256.f, 256.f);
	sprite.m_RotationSpeed = 180.f;
	sprite.m_Orientation = 0.f;
	sprite.m_Color.set(1.f, 0.5f, 0.5f, 1.f);

	return true;
}

void TP2::Update(){
    m_ElapsedTime = EsgiTimer::GetElapsedTimeSince(m_PreviousFrameTime);
	m_PreviousFrameTime = EsgiTimer::GetTimerValue();
	sprite.Process((float)m_ElapsedTime);
}
int mX = 0;
void TP2::MouseMove(int x, int y){
    mX = x;
}
void TP2::Draw(){
    // efface le color buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 1.f, 100.f);
    */
    GLuint programObject = shader.GetProgram();
    glUseProgram(programObject);

    //projectionMatrix = esgiOrtho(0.f, WINDOW_WIDTH, 0.f, WINDOW_HEIGHT, 0.f, 1.f);
    projectionMatrix = esgiPerspective(45.f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 2048.f);
    sprite.viewMatrix = esgiLookAt(vec3(mX, 0, 768), vec3(0, 0, 0), vec3(0, 1, 0));
    //projectionMatrix = esgiFrustum(0.f, WINDOW_WIDTH, 0.f, WINDOW_HEIGHT, 0.f, 10.f);
    GLint projectionUniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
    glUniformMatrix4fv(projectionUniform, 1, 0, &projectionMatrix.I.x);
    //glFrustum(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 1.f, 100.f);

	sprite.Render(programObject);

    glutSwapBuffers();
    glutPostRedisplay();
}
