extern GLuint Rand[512][512][3]; //массив пикселей для процедурной текстуры

void InitRandTex() //генерация массива пикселей для процедурной текстуры 
{
	for(int i = 0; i < 512; i++) 
		for(int j = 0; j < 512; j++)
		{
				Rand[i][j][0] = 100+rand()%55;
				Rand[i][j][1] = 100+rand()%55;
				Rand[i][j][2] = 100+rand()%55;
		}
}

void ModifyRand()
{
	for(int i = 0; i < 512; i++) 
		for(int j = 0; j < 512; j++)
		{
				Rand[i][j][0] = Rand[i][j][0]%5;
				Rand[i][j][1] = Rand[i][j][1]%5;
				Rand[i][j][2] = Rand[i][j][2]%5;
		}
}

GLuint TexGen()
{
	GLuint texture;
	glEnable(GL_DEPTH_TEST);
	for(int i = 0; i < 512; i++)
		for(int j = 0; j < 512; j++)
		{
			Rand[i][j][0] = (GLbyte) Rand[i][j][0];
			Rand[i][j][1] = (GLbyte) Rand[i][j][1];
			Rand[i][j][2] = (GLbyte) Rand[i][j][2];
		}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, Rand);
	return texture;
}