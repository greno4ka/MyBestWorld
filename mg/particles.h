#define bubles 300
extern float movie; //переменная передвижения частиц
extern double bub[bubles][4]; //система частиц
extern const double Rad; //для перевода градусов в радианы

void InitParticle() //инициализация системы частиц
{
	for(int i = 0; i < bubles; i++) 
	{
		int alpha = rand() % 360 ;
		float r = 2.0f*rand() / RAND_MAX;
		bub[i][0] = r*cos(alpha*Rad);				 //х
		bub[i][1] = -4.8+10.0f*rand() / RAND_MAX;  //y
		bub[i][2] = r*sin(alpha*Rad);				 //z
		bub[i][3] = 1.0f + 2.0f * rand() / RAND_MAX; //размер
	}
}

void ParticleSystem() //вывод системы частиц
{
	for (int i = 0; i <bubles; i++)
	{
		glPushMatrix();
		glDisable(GL_COLOR_MATERIAL);
	// Параметры материала 
	GLfloat bsp_a[4] = { 0.3, 0.7, 0.1, 1.0 }; 
	GLfloat bsp_d[4] = { 0.3, 0.7, 0.1, 1.0 };
	GLfloat bsp_s[4] = { 0.3, 0.3, 0.1, 1.0 };
	GLfloat bsp_r = 128;
	// Устанавливаем параметры материала
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bsp_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bsp_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bsp_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &bsp_r);
	
	glEnable(GL_NORMALIZE); // нормализовали нормаль
	glScalef(0.2,0.2,0.2); // масштаб пузырьков
	
	glTranslatef(bub[i][0], bub[i][1]+movie, bub[i][2]);
	glutSolidSphere(0.1,10,10);
	glPopMatrix();
		glEnable(GL_COLOR_MATERIAL);
	}
}