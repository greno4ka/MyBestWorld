void InitLight(){
	// Устанавливаем цвет источника
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // Значения диффузного света
	GLfloat LightAmbient[]= { 0.3f, 0.3f, 0.3f, 1.0f }; // Значения фонового света
	GLfloat LightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	
	glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDiffuse);
	glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmbient);
	glLightfv(GL_LIGHT0,GL_SPECULAR,LightSpecular);

	
	// Устанавливаем источник освещения 
	GLfloat LightPosition[]= {2.0f,4.0f,2.0f,1};
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,LightPosition);
	//Включаем освещение
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);					
}