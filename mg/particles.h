#define bubles 300
extern float movie; //���������� ������������ ������
extern double bub[bubles][4]; //������� ������
extern const double Rad; //��� �������� �������� � �������

void InitParticle() //������������� ������� ������
{
	for(int i = 0; i < bubles; i++) 
	{
		int alpha = rand() % 360 ;
		float r = 2.0f*rand() / RAND_MAX;
		bub[i][0] = r*cos(alpha*Rad);				 //�
		bub[i][1] = -4.8+10.0f*rand() / RAND_MAX;  //y
		bub[i][2] = r*sin(alpha*Rad);				 //z
		bub[i][3] = 1.0f + 2.0f * rand() / RAND_MAX; //������
	}
}

void ParticleSystem() //����� ������� ������
{
	for (int i = 0; i <bubles; i++)
	{
		glPushMatrix();
		glDisable(GL_COLOR_MATERIAL);
	// ��������� ��������� 
	GLfloat bsp_a[4] = { 0.3, 0.7, 0.1, 1.0 }; 
	GLfloat bsp_d[4] = { 0.3, 0.7, 0.1, 1.0 };
	GLfloat bsp_s[4] = { 0.3, 0.3, 0.1, 1.0 };
	GLfloat bsp_r = 128;
	// ������������� ��������� ���������
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bsp_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bsp_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bsp_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &bsp_r);
	
	glEnable(GL_NORMALIZE); // ������������� �������
	glScalef(0.2,0.2,0.2); // ������� ���������
	
	glTranslatef(bub[i][0], bub[i][1]+movie, bub[i][2]);
	glutSolidSphere(0.1,10,10);
	glPopMatrix();
		glEnable(GL_COLOR_MATERIAL);
	}
}