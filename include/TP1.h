#ifndef _TP1_H
#define _TP1_H

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "config.h"
#include "EsgiSprite.h"
#include "EsgiShader.h"
#include "common/EsgiTga.h"
#include "common/EsgiTimer.h"
#include "common/matrix.h"
#include "common/vector.h"

class TP1{
public:
    TP1();
    ~TP1();
    void Update();
    void Draw();
    void MouseMove(int x, int y);
    bool Init();
private:
    EsgiSprite sprite;
    double m_PreviousFrameTime;
    double m_ElapsedTime;
    EsgiShader shader;
    mat4 projectionMatrix;
};

#endif
