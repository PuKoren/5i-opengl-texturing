// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_TGA
#define ESGI_TGA

// --- Includes --------------------------------------------------------------

#include <GL/glew.h>
#include <cstdio>

// esgiTexture ---

struct EsgiTexture
{
    GLuint width; 
    GLuint height;
    GLenum datatype;
    GLenum format;
    GLint  internalFormat;
    GLuint id;

    GLubyte *texels;
};

// EsgiTgaHeader ---
// librement inspire de http://tfc.duke.free.fr/coding/tga.html
// Le taille de la structure doit être de 18 octets
// Il faut donc modifier l'alignement de base pour cette structure a l'octet pres

#pragma pack(push, 1)
struct EsgiTgaHeader
{
  GLubyte id_length;          /* size of image id */
  GLubyte colormap_type;      /* 1 is has a colormap */
  GLubyte image_type;         /* compression type */
  // sans le pragma pack a 1, un octet de bourrage aurait ete insere ici!
  short	cm_first_entry;       /* colormap origin */
  short	cm_length;            /* colormap length */
  GLubyte cm_size;            /* colormap size */
  // sans le pragma pack a 1, un octet de bourrage aurait ete insere ici!
  short	x_origin;             /* bottom left x coord origin */
  short	y_origin;             /* bottom left y coord origin */

  short	width;                /* picture width (in pixels) */
  short	height;               /* picture height (in pixels) */

  GLubyte pixel_depth;        /* bits per pixel: 8, 16, 24 or 32 */
  GLubyte image_descriptor;   /* 24 bits = 0x00; 32 bits = 0x80 */
};
#pragma pack(pop)

void GetTextureInfo(const EsgiTgaHeader *header, EsgiTexture *texinfo);
void ReadTGA8bits(FILE *fp, const GLubyte *colormap, EsgiTexture *texinfo);
void ReadTGAgray8bits(FILE *fp, EsgiTexture *texinfo);
void ReadTGA16bits(FILE *fp, EsgiTexture *texinfo);
void ReadTGAgray16bits(FILE *fp, EsgiTexture *texinfo);
void ReadTGA24bits(FILE *fp, EsgiTexture *texinfo);
void ReadTGA24bitsRLE (FILE *fp, EsgiTexture *texinfo);
void ReadTGA32bits(FILE *fp, EsgiTexture *texinfo);
struct EsgiTexture* esgiReadTGAFile(const char *filename);

#endif // ESGI_TGA