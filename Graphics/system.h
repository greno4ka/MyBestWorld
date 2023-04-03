extern double x_angle, y_angle;
extern int mouse_x,mouse_y,mouse_button,mouse_state;
extern bool fog, mballs;

extern const double PI;
extern const double Rad; //дл€ перевода градусов в радианы
extern float cx,cy,cz,fi,psy,fi_rad,psy_rad; //переменные положени€ наблюдател€ (камеры)
GLint cur_x = 512, cur_y = 384; //расположение курсора на экране

bool fullscreen=true;

void mouseClick(int button,int state,int x,int y)
{
	mouse_button = button;
	mouse_state = state;
	mouse_x = x;
	mouse_y = y;
}

void mouseMotion(int x,int y)
{
	fi  += (float)(x - cur_x) / 10.0f;
	psy += (float)(y - cur_y) / 10.0f;
	cur_x = x;
	cur_y = y; 
	glutPostRedisplay();
	if (abs(512 - x) > 5 || abs(384 - y) > 5)
	{
		glutWarpPointer(512, 384);
		cur_x = 512;
		cur_y = 384;
	}
}

void keyboard (unsigned char key, int x, int y)
{
	float last_cx=cx,
			last_cy=cy,
				last_cz=cz;
	float v = 0.3f;
	if (key == 27)
	{
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		exit(0);
	}

	if (key == 'g' || key == 'G')
	{
		if(!fog)                   // Ќажата ли клавиша "G"?
       fog=TRUE;                         // gp устанавливаем в TRUE
		else fog=FALSE;                        // ≈сли да, gp установить в FALSE
	}	
	
	if (key == 'm' || key == 'M')
	{
		if(!mballs)                   // Ќажата ли клавиша "G"?
       mballs=TRUE;                         // gp устанавливаем в TRUE
		else mballs=FALSE;                        // ≈сли да, gp установить в FALSE
	}
	
	if (key == 'f' || key == 'F') //toggle screenmode
        if(!fullscreen) {
            glutFullScreen();
            fullscreen = true;
			glutSetCursor(GLUT_CURSOR_NONE);
        } else if(fullscreen) {
            glutReshapeWindow(800, 600);
            glutPositionWindow(0,0);
            fullscreen = false;
        }
	if (key == 'w' || key == 'W')
	{
		fi_rad = fi * Rad;
		psy_rad = psy * Rad;
		cx += v * sin(fi_rad) * cos(psy_rad);
		cy -= v * sin(psy_rad);
		cz -= v * cos(fi_rad) * cos(psy_rad);
	}

	if (key == 's' || key == 'S')
	{
		fi_rad = fi * Rad;
		psy_rad = psy * Rad;
		cx -= v * sin(fi_rad) * cos(psy_rad);
		cy += v * sin(psy_rad);
		cz += v * cos(fi_rad) * cos(psy_rad);
	}

	if (key == 'a' || key == 'A')
	{
		fi_rad = fi * Rad + PI / 2.0;
		psy_rad = psy * Rad;
		cx -= v * sin(fi_rad ) * cos(psy_rad);
		cy += v * sin(psy_rad);
		cz += v * cos(fi_rad ) * cos(psy_rad);
	}

	if (key == 'd' || key == 'D')
	{
		fi_rad = fi * Rad + PI / 2.0;
		psy_rad = psy * Rad;
		cx += v * sin(fi_rad ) * cos(psy_rad);
		cy -= v * sin(psy_rad);
		cz -= v * cos(fi_rad ) * cos(psy_rad);
	}

	if (cy<=-0.18*5+v) cy=last_cy;
	if (cy>=5-v) cy=last_cy;
	if (cx<=-5+v) cx=last_cx;
	if (cx>=5-v) cx=last_cx;
	if (cz>=5-v) cz=last_cz;
	if (cz<=-5+v) cz=last_cz;
	
	glutPostRedisplay();
}


void reshape (int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluPerspective(45.0, (GLfloat)w/h, 0.01, 100);
    glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
}