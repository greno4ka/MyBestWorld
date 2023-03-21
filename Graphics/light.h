void InitLight(){
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat LightAmbient[]= { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat LightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	
	glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDiffuse);
	glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmbient);
	glLightfv(GL_LIGHT0,GL_SPECULAR,LightSpecular);

    GLfloat LightPosition[]= { 2.0f, 4.0f, 2.0f, 1.0f };
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,LightPosition);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);					
}
