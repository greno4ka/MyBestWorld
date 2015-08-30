#include <GL\freeglut.h>
#include <GL\glu.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include "BSpline.h"
#include "formats.h"
#include "reflection.h" // просто матрица
#include "shadow.h"		// тоже просто матрица
#include "math3d.h"
#include "system.h"
#include "particles.h"
#include "texgen.h"
#include "light.h"

const double PI = 3.14159265358979323846;
const double Rad = PI / 180.0f; // для перевода градусов в радианы
GLint X = 800, Y = 600;
int mouse_x = 0,
	mouse_y = 0,
	mouse_button = -1,
	mouse_state = GLUT_UP;

float cx = 2.0f,
 	  cy = 1.0f,
	  cz = 2.0f,
	  fi = -45.0f,
	  psy = -20.0f,
	  fi_rad = 0.0f,
	  psy_rad = 0.0f;

/// ***** для текcтурок *****
int tex_width,
    tex_height;
unsigned char *t1=NULL,
			  *t2=NULL;
GLuint texture[2],butt;

/// ***** зеркало *****
float reflection_matrix [4][4]={0};
float plane_point[3] ={-5.0,0.0,0.0};
float plane_normal[3] = {1.0,0.0,0.0};

///***** туман *****
bool   fog=FALSE;								  // G Нажата?
GLuint filter;									  // Используемый фильтр для текстур 
GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR }; // Хранит три типа тумана
GLuint fogfilter= 0;							  // Тип используемого тумана
GLfloat fogColor[4]= {0.5f, 0.5f, 0.5f, 1.0f};	  // Цвет тумана

///***** система частиц *****
#define bubles 300
float movie=0; //переменная передвижения частиц
double bub[bubles][4]; //система частиц

///***** процедурная текстура *****
static GLuint mytex; //имя процедурной текстуры
static GLuint Rand[512][512][3]; //массив пикселей для процедурной текстуры

/// ***** игра теней *****
GLfloat SM[4][4]; //матрица проецирования
GLfloat ShadowPlane[4] = {0,1,0,0.89}; // уравнение плоскости
GLfloat LightPos[4] = {2.0f, 5.0f, 2.0f, 1.0f}; //расположение источника света

bool mballs=false; // избавиться от дурацких шаров

GLuint objectList;
GLuint  square;

// Количество точек в аппроксимации контура
int N = 100;

double2 ControlPoints[] = {
	//bottom
	double2(0,-0.005),
	double2(0.5,-0.005),
	double2(0.52,-0.005),
	double2(0.52,0.045),
	
	double2(0.52,1.80),
	double2(0.53,2.40),
	double2(0.22,2.50),
	double2(0.21,2.70),

	double2(0.21,3.40),
	double2(0.24,3.40),
	double2(0.24,3.40),
	double2(0.24,3.550),
	double2(0.21,3.550),
	// top
	double2(0.21,3.650),
	double2(0.21,3.650),
	double2(0.2,3.650),
	double2(0.0,3.650),
	double2(0.0,3.650),
};

struct Metaball
{
	double x,y,z;
	double mas; 
	double speed; 
} mb[9];


inline void glVertex(const double2 v) {glVertex2d(v.x,v.y);}
inline void glVertex(const double3 v) {glVertex3d(v.x,v.y,v.z);}
inline void glNormal(const double3 v) {glNormal3d(v.x,v.y,v.z);}
inline double3 RotateY(double angle,const double2 v)
{return double3(cos(angle)*v.x,v.y,sin(angle)*v.x);}

GLuint GenSquare() {
	glPushMatrix();
	
	GLuint list = glGenLists(1);
	glNewList(list,GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < 512; i+=2) 
		{
			for(int j = 0; j < 512; j++)
			{
				glColor3ub((5-Rand[i+1][j][0])*(rand()%255),(5-Rand[i+1][j][0])*(rand()%255),(5-Rand[i+1][j][0])*(rand()%255));
				glVertex3f(i/50.0f,Rand[i][j][0]/80.0f,j/50.0f);
				glColor3ub((5-Rand[i+1][j][0])*(rand()%255),(5-Rand[i+1][j][0])*(rand()%255),(5-Rand[i+1][j][0])*(rand()%255));
				glVertex3f((i+1)/50.0f,Rand[i+1][j][0]/80.0f,j/50.0f);
			}
			for(int j = 511; j >= 0; j--)
			{
				glColor3ub((5-Rand[i+1][j][0])*(rand()%255),(5-Rand[i+1][j][0])*(rand()%255),(5-Rand[i+1][j][0])*(rand()%255));
				glVertex3f((i+1)/50.0f,Rand[i+1][j][0]/80.0f,j/50.0f);
				glColor3ub((5-Rand[i+1][j][0])*(rand()%255),(5-Rand[i+1][j][0])*(rand()%255),(5-Rand[i+1][j][0])*(rand()%255));
				glVertex3f((i+2)/50.0f,Rand[i+2][j][0]/80.0f,j/50.0f);
			}
		}		
	glEnd();
	glEndList();	
	glPopMatrix();
	return list;
}

void DrawSquare()
{
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable (GL_DEPTH_TEST);
	
	glTranslatef(-5,4.92,-5);
	glCallList(square);
	glEnable(GL_COLOR_MATERIAL);
    glFlush();
	glPopMatrix();
}


inline double f(double x1, double y1, double z1, double x2, double y2, double z2, double mas) // основная функция для меташаров (здесь 1 / r^2)
{return  mas / (((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)) + ((z1 - z2) * (z1 - z2)) + 0.00000001);}

double threshold = 100; // пороговое значение для меташаров

double binary_find(double low, double high, double x) // рекурсивная функция метода деления пополам
{
	if (high - low > 0.001)
	{
		double temp = 0;
		double average = (low + high) * 0.5;
		for (int i = 0; i < 9; i++)
			temp += f(mb[i].x, mb[i].y, mb[i].z, x, average, 0, mb[i].mas);
		   
		if (temp < threshold) return binary_find(low, average,  x);
		else return binary_find(average, high, x);
	} else  return (low + high) * 0.5;
}

void DrawMetaball()
{
  GLUquadricObj * qobj = gluNewQuadric();
  double temp_x = 0; // начальная координата по х (оси, на которой строятся меташары)
  double radius = 0; // радиус усеченного конуса (по сути координата y, найденная бинарным поиском)
  double radius_last = 0; // радиус предыдущего усеченного конуса
  
  for (int i = 0; i < 2000; i++)
  {
	  temp_x += 0.005;
	  radius_last = radius;
	  radius = binary_find(0, 100, temp_x);
	  if (radius < 0.01) // устранение бага с точками
		  radius = 0;
	  
	  glPushMatrix();
	  glEnable(GL_COLOR_MATERIAL);
	  glEnable (GL_LIGHTING);
	  
	  glRotatef(90, 0, 1, 0);
	  glTranslatef(temp_x-5, 0.5f, -2.0f);
	  glRotatef(90.0f, 0, 1, 0);
	  glColor3ub(30, 30, 30);
	  gluCylinder(qobj, radius_last, radius, 0.01, 15, 15);
	  glPopMatrix();
  }
}

BSpline<double2> bsp(10, OpenBasis, sizeof(ControlPoints)/sizeof(ControlPoints[0]), 3, 0, 1);

void DrawBSP(){	
	glPushMatrix();
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.05);
	// Параметры материала 
	GLfloat bsp_a[4] = { 0.0, 0.01, 0.0, 0.8 }; 
	GLfloat bsp_d[4] = { 0.0, 0.05, 0.0, 0.8 };
	GLfloat bsp_s[4] = { 0.0, 0.1, 0.0, 0.8 };
	GLfloat bsp_r = 128;
	// Устанавливаем параметры материала
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bsp_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bsp_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bsp_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &bsp_r);
	
	glEnable(GL_NORMALIZE); // нормализовали нормаль
	glScalef(0.9,0.9,0.9); // масштаб бутылки
	glTranslatef(0.0,-1.0,0.0);
	glCallList(objectList);
	glEnable(GL_COLOR_MATERIAL);
    glFlush();
	glPopMatrix();
}

void DrawLamp(){	
	glPushMatrix();
	
	glEnable(GL_LIGHTING); 
	// Параметры материала 
	GLfloat bsp_a[4] = { 3, 3, 3, 1.0 }; 
	GLfloat bsp_d[4] = { 3, 3, 3, 1.0 };
	GLfloat bsp_s[4] = { 3, 3, 3, 1.0 };
	GLfloat bsp_r = 128;
	// Устанавливаем параметры материала
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bsp_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bsp_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bsp_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &bsp_r);
	
	glEnable(GL_NORMALIZE); // нормализовали нормаль
	glTranslatef(2,5,2);
	glutSolidSphere(0.1,20,20);
	glEnable(GL_COLOR_MATERIAL);
    //glFlush();
	glPopMatrix();
}

void DrawMirror()
{
	glPushMatrix();
	glScaled(5,5,5);
	float glass_diff[4]= {0.2f, 0.2f, 0.2f, 1.0f};
	float glass_spec[4] = {0.8f, 0.8f, 0.8f, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, glass_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, glass_spec);
	glColor4f(1, 1 ,1, 0.3f);
		
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glEnable(GL_TEXTURE_2D);
	
	glBegin(GL_QUADS);
			glNormal3f(1,0,0);
			glTexCoord2f(0,0);
			glVertex3f(-1,-1,1);
			glTexCoord2f(1,0);
			glVertex3f(-1,-1,-1);
			glTexCoord2f(1,1);
			glVertex3f(-1,1,-1);
			glTexCoord2f(0,1);
			glVertex3f(-1,1,1);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void DrawCornellBox()
{
	glPushMatrix();
	glEnable(GL_NORMALIZE);
	glScaled(5,5,5);
	glDisable(GL_COLOR_MATERIAL);
	// Параметры материала 
	GLfloat bsp_a[4] = { 0.3, 0.3, 0.3, 1 }; 
	GLfloat bsp_d[4] = { 0.5, 0.5, 0.5, 1 };
	GLfloat bsp_s[4] = { 0.0, 0.0, 0.0, 1 };
	GLfloat bsp_r = 1;
	// Устанавливаем параметры материала
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bsp_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bsp_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bsp_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &bsp_r);

	glEnable (GL_LIGHTING);
	glEnable (GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glEnable(GL_TEXTURE_2D);
	
	double h=0.1;		
	glBegin(GL_QUADS);
	glNormal3f(1,0,0);
		// передняя
	for (double i=-1.0;i<1.0-h;i+=h)
		for (double j=-1.0;j<1.0-h;j+=h) 
			{
			glTexCoord2f(0.0f, 0.0f);	glVertex3d(i,j,1.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex3d(i+h,j,1.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex3d(i+h,j+h,1.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex3d(i,j+h,1.0f);
			};
		// задняя
		for (double i=-1.0;i<1.0-h;i+=h)
		for (double j=-1.0;j<1.0-h;j+=h) 
			{
			glTexCoord2f(0.0f, 0.0f);	glVertex3d(i,j,-1.0f);
			glTexCoord2f(1.0f, 0.0f);	glVertex3d(i+h,j,-1.0f);
			glTexCoord2f(1.0f, 1.0f);	glVertex3d(i+h,j+h,-1.0f);
			glTexCoord2f(0.0f, 1.0f);	glVertex3d(i,j+h,-1.0f);
			};	
		// верхняя
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		// нижняя
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		// правая
		h=0.2;
		for (double i=-1.0;i<1.0;i+=h)
		for (double j=-1.0;j<1.0;j+=h) 
			{
			glTexCoord2f(1.0f, 0.0f);	glVertex3d(1.0f,i,j);
			glTexCoord2f(1.0f, 1.0f);	glVertex3d(1.0f,i+h,j);
			glTexCoord2f(0.0f, 1.0f);	glVertex3d(1.0f,i+h,j+h);
			glTexCoord2f(0.0f, 0.0f);	glVertex3d(1.0f,i,j+h);
			};	
		// левая
		//glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		//glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		//glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		//glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
	glEnd();	

	// стол
	glBindTexture(GL_TEXTURE_2D, mytex);		
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -0.18f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -0.18f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, -0.18f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -0.18f,  1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);	

	glPopMatrix();
}

unsigned char *LoadTrueColorBMPFile(const char *path,int *width,int *height)
{
	unsigned char *result = NULL;
    FILE *in = fopen(path,"rb");
	if(!in)
		return NULL;
    BMPHeader hdr;
    fread(&hdr,sizeof(hdr),1,in);
    if(hdr.type != 0x4D42)           // Not a bitmap file at all
		return NULL;
	BMPInfoHeader	infoHdr;
	fread(&infoHdr,sizeof(infoHdr),1,in);
	if(infoHdr.bitsPerPixel != 24)   // Not a truecolor bitmap
		return NULL;
	if(infoHdr.compression)          // Compressed bitmap
		return NULL;
	if((result = new unsigned char[infoHdr.width*infoHdr.height*3]) == NULL)
		return NULL;
    fseek(in,hdr.offset-sizeof(hdr)-sizeof(infoHdr),SEEK_CUR);   
	unsigned char *dst = result;
	for(int y=0;y<infoHdr.height;y++) {
		for(int x=0;x<infoHdr.width;x++) {
			dst[2] = fgetc(in);
			dst[1] = fgetc(in);
			dst[0] = fgetc(in);
			dst += 3;
		}
        for(int x=0;x<((4-(infoHdr.width&3))&3);x++)  // Skip alignment bytes
           fgetc(in);
    }
    fclose(in);
	*width = infoHdr.width;
	*height = infoHdr.height;
	return result;
}


/// специальная загрузка для billboard
unsigned char *ConstructTexture(int *w,int *h)
{
	int width1,height1;
	unsigned char *tex1 = LoadTrueColorBMPFile("Textures/butterfly1.bmp",&width1,&height1);
	if(!tex1)
		return NULL;
	int width2,height2;
	unsigned char *tex2 = LoadTrueColorBMPFile("Textures/butterfly2.bmp",&width2,&height2);
	if(!tex2) {
		delete[] tex1;
		return NULL;
	}
	if(width1 != width2 || height1 != height2) {
		delete[] tex1;
		delete[] tex2;
		return NULL;
	}
	unsigned char *result = new unsigned char [width1*height1*4];
	if(result != NULL) {
		for(int i = 0;i<width1*height1;i++) {
			result[4*i] = tex1[3*i];
			result[4*i + 1] = tex1[3*i + 1];
			result[4*i + 2] = tex1[3*i + 2];
			result[4*i + 3] = (unsigned char)(((int)tex2[3*i] + (int)tex2[3*i + 1] + (int)tex2[3*i + 2])/3);
		}
	}	
	delete[] tex1;
	delete[] tex2;
	*w = width1;
	*h = height1;
	return result;
}

#define BUTTS 4 
double Butts[BUTTS][6] = // (x,y,z, высота, ширина, расстояние до камеры)
{ 
	{-1, 3, -2, 0.3, 0.3, 0},
	{-1, 3, 1, 0.3, 0.3, 0},
	{ 1, 3, 2, 0.3, 0.3, 0},
	{ 1, 3, -1, 0.3, 0.3, 0}
}; 

void CalcDestButts()
{
	for (int i = 0; i < BUTTS; ++i)
	{
		float x = cx - Butts[i][0];
		float y = cy - Butts[i][1];
		float z = cz - Butts[i][2];
		Butts[i][5] = sqrt(x*x+y*y+z*z);
	}
}

void SortButts(int L, int R) // сортировка биллбоардов
{	
	while (L < R)
    {
		double x = Butts[L][5];
		int i = L, j = R;
		while (i <= j)
		{
			while (Butts[i][5] < x) i++;
			while (x < Butts[j][5]) j--;
			if (i <= j)
			{
				for (int m = 0; m < 6; m++)
				{
					float b = Butts[i][m]; 
					Butts[i][m] = Butts[j][m]; 
					Butts[j][m] = b;
				}
				i++; 
				j--;
			}
		}
		if ((j - L) > (R - i))
		{
			SortButts(i, R);
			R = j;
		}
		else
		{
			SortButts(L, j);
			L = i;
		}
	}
}

void Picture(GLfloat x, GLfloat y, GLfloat z, float size1, float size2, GLint texture) //рендеринг биллбоарда
{
	glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(-fi,0,1,0);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, texture);
        glColor4f(0.5, 0.5, 0.0, 1);
    glBegin(GL_QUADS);     
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-size1, -size2, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( size1, -size2, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( size1,  size2, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-size1,  size2, 0);
    glEnd();  
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void AddButts() // добавление массива биллбоардов
{
	for (int i = BUTTS - 1; i >= 0; i--)
	{
		Picture(Butts[i][0], Butts[i][1], Butts[i][2], Butts[i][3], Butts[i][4], butt);
	}
}

bool InitTex(){	
	if((t1 = LoadTrueColorBMPFile("Textures/glass.bmp",&tex_width,&tex_height)) == NULL) return false;
	if((t2 = LoadTrueColorBMPFile("Textures/cmc.bmp",&tex_width,&tex_height)) == NULL)	  return false;	
	glGenTextures(3,texture); // Создаем текстурный объект

	glPixelStorei(GL_UNPACK_ALIGNMENT,1); // Загружаем текстуру
	glBindTexture(GL_TEXTURE_2D,texture[0]); // Активизируем текстурный  объект
	gluBuild2DMipmaps(GL_TEXTURE_2D,  
			          3,                     // Формат текстуры
			          tex_width,tex_height,  // Размер текстуры
			          GL_RGB,                // Формат исходных данных
			          GL_UNSIGNED_BYTE,      // Тип исходных данных
				      t1);             // Указатель на исходные данные */
	delete[] t1;	// Освобождаем память, в которой хранилась текстура

	

//то же самое для второй из массива
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	gluBuild2DMipmaps(GL_TEXTURE_2D,  
			          3,                     // Формат текстуры
			          tex_width,tex_height,  // Размер текстуры
			          GL_RGB,                // Формат исходных данных
			          GL_UNSIGNED_BYTE,      // Тип исходных данных
				      t2);             // Указатель на исходные данные */
	delete[] t2;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	return true;
}    

bool firstInit(void)
{ 
	for(int i = 0;i<sizeof(ControlPoints)/sizeof(ControlPoints[0]);i++)
	bsp.ControlPoint(i) = ControlPoints[i];
	bsp.SetTesselation(200);

	ReflectionMatrix(reflection_matrix,plane_point,plane_normal);

    if (!InitTex()) return FALSE;

    InitLight();
	/// бутылка
	objectList = glGenLists(1);
	glNewList(objectList,GL_COMPILE);
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glColor3f(1,1,1);
	// Нижняя розетка треугольников
	glBegin(GL_TRIANGLES);
	for(int i = 0;i<N;i++)
	{
		glNormal(RotateY(2*PI*(i + 0.5)/N,GetNormal(bsp,0)));
		glVertex3d(0,bsp.GetPoint(0).y,0);
		double phi = 2*PI*i/N;
		glNormal(RotateY(phi,GetNormal(bsp,1)));
		glVertex(RotateY(phi,bsp.GetPoint(1)));
		int i2 = (i < N-1)?(i+1):(0);
		double phi2 = 2*PI*i2/N;
		glNormal(RotateY(phi2,GetNormal(bsp,1)));
		glVertex(RotateY(phi2,bsp.GetPoint(1)));
	}
	glEnd();
		
	// Средняя часть объекта
	for(int j = 1;j<bsp.GetTesselation() - 2;j++) {
		glBegin(GL_QUAD_STRIP);
		for(int i = 0;i<=N;i++) {
			double phi = (i < N)?(2*PI*i/N):(0);
			glNormal(RotateY(phi,GetNormal(bsp,j)));
			glVertex(RotateY(phi,bsp.GetPoint(j)));
			glNormal(RotateY(phi,GetNormal(bsp,j+1)));
			glVertex(RotateY(phi,bsp.GetPoint(j+1)));
		}
		glEnd();
	}

	// Верхняя розетка треугольников
	glBegin(GL_TRIANGLES);
	int n2 = bsp.GetTesselation() - 1;
	for(int i = 0;i<N;i++) {
		double phi = 2*PI*i/N;
		glNormal(RotateY(phi,GetNormal(bsp,n2 - 1)));
		glVertex(RotateY(phi,bsp.GetPoint(n2 - 1)));
		int i2 = (i < N-1)?(i+1):(0);
		double phi2 = 2*PI*i2/N;
		glNormal(RotateY(phi2,GetNormal(bsp,n2 - 1)));
		glVertex(RotateY(phi2,bsp.GetPoint(n2 - 1)));
		glNormal(RotateY(2*PI*(i + 0.5)/N,GetNormal(bsp,n2)));
		glVertex3d(0,bsp.GetPoint(0).y,0);
	}
	glEnd();
	glEndList();

	glEnable(GL_DEPTH_TEST); // потом мы будем рисовать освещённую сцену поверх неосвещённой
	glDepthFunc(GL_LEQUAL);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL); // теперь цвет объектов будет сохраняться и при освещении
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);
	
///***** меташары *****
	for (int i = 0; i < 9; i++)
	{
		mb[i].x = i-4.5;
		mb[i].y = 0;
		mb[i].z = 0;
		mb[i].mas = 10;
		mb[i].speed = ((rand() % 200) - 100) * 0.01 + 0.03;
		if (mb[i].speed < 0.1)
			mb[i].speed = 0.1;
	}

	///*****бабочки*****
unsigned char *tex_bits = NULL;
	int tex_width,tex_height;
	if((tex_bits = ConstructTexture(&tex_width,&tex_height)) == NULL)
		return false;
	glGenTextures(1,&butt);
	glBindTexture(GL_TEXTURE_2D,butt);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
		         0,
			     4,
			     tex_width,tex_height,
			     0,
			     GL_RGBA,
			     GL_UNSIGNED_BYTE,tex_bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return TRUE;
}

void display (void)
{
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
glClearColor(0,0,0,1.0f);      
if (fog)
{
glClearColor(0.5f,0.5f,0.5f,1.0f);      // Будем очищать экран, заполняя его цветом тумана. ( Изменено )
glEnable(GL_FOG);                       // Включает туман (GL_FOG)
glFogi(GL_FOG_MODE, fogMode[1]);// Выбираем тип тумана
glFogfv(GL_FOG_COLOR, fogColor);        // Устанавливаем цвет тумана
glFogf(GL_FOG_DENSITY, 0.10f);          // Насколько густым будет туман
glHint(GL_FOG_HINT, GL_NICEST);      // Вспомогательная установка тумана
glFogf(GL_FOG_START, 0.1f);             // Глубина, с которой начинается туман
glFogf(GL_FOG_END, 15.0f);               // Глубина, где туман заканчивается.
}
else 
glDisable(GL_FOG);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	
	glLoadIdentity();
	glRotatef(fi, 0.0f, 1.0f, 0.0f);
	glRotatef(psy, (float) cos(fi * Rad), 0.0f, (float) sin(fi * Rad));
	glTranslatef(-cx, -cy, -cz);
	glPushMatrix();
	DrawCornellBox();
	DrawLamp();
	ParticleSystem();
	DrawBSP();
	AddButts();
	DrawSquare();
	if(mballs) DrawMetaball();
	
	/// Нарисовать тень
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ShadowMatrix(SM, ShadowPlane, LightPos);
	glPushMatrix();
	glMultMatrixf((GLfloat *)SM);
	DrawBSP();
	ParticleSystem();
	if(mballs) DrawMetaball();
	glPopMatrix();
	// Закончили рисовать тень
 
	//рисуем зеркало
	// Помечаем в буфере трафарета пикселы отражающей поверхности
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS,1,0);
	glStencilOp(GL_ZERO,GL_ZERO,GL_REPLACE);
	glDepthMask(GL_FALSE);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	
	DrawMirror();
	
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	
	// Рисуем отраженные объекты
	glStencilFunc(GL_EQUAL,0x1,0xffffffff);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glStencilFunc(GL_EQUAL,1,1);
	
	glPushMatrix();
	glMultMatrixf((float*)reflection_matrix);
	DrawCornellBox();
	ParticleSystem();
	DrawBSP();
	AddButts();
	DrawSquare();
	if(mballs) DrawMetaball();
	glPopMatrix();
	
	glPushMatrix();
	glMultMatrixf((float*)reflection_matrix);
	glMultMatrixf((GLfloat *)SM);
	DrawBSP();
	ParticleSystem();
	if(mballs) DrawMetaball();
	glPopMatrix();

	glDisable(GL_STENCIL_TEST);
	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawMirror();
	glDisable(GL_BLEND); 

	movie+=0.1f;
	for (int i = 0; i<bubles; i++)
	{
		if (bub[i][1] + movie > 5)
			bub[i][1] -=  9;
	}

	glPopMatrix();
	glutSwapBuffers();
}

void idle (void)
{
	 for (int i = 0; i < 9; i++)
	{
		mb[i].x += mb[i].speed * 0.01 * 20;
		if ((mb[i].x < -4.7) || (mb[i].x > 9.7))
		mb[i].speed = -mb[i].speed; // отражение от стенок
	}	 	
  glutPostRedisplay();
}

int main (int argc, char **argv)
{
  srand(time (NULL));
  InitParticle(); 
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  glutInitWindowSize(X, Y);
  glutCreateWindow ("Computer graphics");
  glutFullScreen();
  glutSetCursor(GLUT_CURSOR_NONE);
  if (!firstInit()) return 1;
  InitRandTex();
  mytex = TexGen();
  ModifyRand();
	square = GenSquare();
  glutMouseFunc(mouseClick);
  glutPassiveMotionFunc(mouseMotion);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle); 
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  
	glDeleteLists(square,1);
  return 0;
}

