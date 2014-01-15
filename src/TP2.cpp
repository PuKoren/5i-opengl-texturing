#include "TP2.h"

TP2::TP2(){
    sprite.indices = new unsigned short[8];
    sprite.indices[0] = 0;
    sprite.indices[1] = 1;
    sprite.indices[2] = 4;

    sprite.indices[3] = 2;
    sprite.indices[4] = 3;
    sprite.indices[5] = 3;
    sprite.indices[6] = 4;
    sprite.indices[7] = 0;

    m_ElapsedTime = 0;
    m_PreviousFrameTime = 0;

	logo = NULL;
}

TP2::~TP2(){
    if(sprite.m_TextureId)
    {
        glDeleteTextures(1, &sprite.m_TextureId);
    }
	delete logo;
	shader.Destroy();
    delete[] sprite.indices;
}

float eyeX, eyeY, eyeZ = 0;
float radius = 768;

bool TP2::Init(){
    if(shader.LoadVertexShader("../resources/simpleSprite.vert") && shader.LoadFragmentShader("../resources/simpleSprite.frag")){
	    shader.Create();
    }else{
        return false;
    }

	// camera a l'origine
	sprite.viewMatrix.Identity();

    logo = esgiReadTGAFile("../resources/kt_rock_1c.tga");
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

    eyeZ = radius;
	return true;
}

void TP2::Update(){
    m_ElapsedTime = EsgiTimer::GetElapsedTimeSince(m_PreviousFrameTime);
	m_PreviousFrameTime = EsgiTimer::GetTimerValue();
	sprite.Process((float)m_ElapsedTime);
}

void TP2::MouseMove(int x, int y){
    float relativeX = (((float)x/(float)WINDOW_WIDTH))*ESGI_PI;
    //float relativeY = (((float)y/(float)WINDOW_HEIGHT))*ESGI_PI;
    eyeX = 0 + radius*sin(relativeX);
    //eyeY = 0 + radius*sin(relativeY);
    eyeZ = 0 + radius*cos(relativeX);
}
void TP2::Draw(){
    // efface le color buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 1.f, 100.f);
    */
    GLuint programObject = shader.GetProgram();
    glUseProgram(programObject);

    //projectionMatrix = esgiOrtho(0.f, WINDOW_WIDTH, 0.f, WINDOW_HEIGHT, 0.f, 1.f);
    projectionMatrix = esgiPerspective(45.f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 2048.f);
    sprite.viewMatrix = esgiLookAt(vec3(eyeX, eyeY, eyeZ), vec3(0, 0, 0), vec3(0, 1, 0));
    //projectionMatrix = esgiFrustum(0.f, WINDOW_WIDTH, 0.f, WINDOW_HEIGHT, 0.f, 10.f);
    GLint projectionUniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
    glUniformMatrix4fv(projectionUniform, 1, 0, &projectionMatrix.I.x);
    //glFrustum(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, 1.f, 100.f);

	sprite.Render(programObject);

    glutSwapBuffers();
    glutPostRedisplay();
}
