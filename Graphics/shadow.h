void ShadowMatrix(GLfloat SM[4][4], GLfloat plane[4], GLfloat Lightpos[4]) //задание матрицы проецирования для построения тени
{
	GLfloat dot = plane[0]*Lightpos[0] + 
			      plane[1]*Lightpos[1] + 
				  plane[2]*Lightpos[2] + 
				  plane[3]*Lightpos[3];

	SM[0][0] = dot-plane[0]*Lightpos[0];
	SM[1][0] =    -plane[1]*Lightpos[0];
	SM[2][0] =    -plane[2]*Lightpos[0];
	SM[3][0] =    -plane[3]*Lightpos[0];
	SM[0][1] =    -plane[0]*Lightpos[1];
	SM[1][1] = dot-plane[1]*Lightpos[1];
	SM[2][1] =    -plane[2]*Lightpos[1];
	SM[3][1] =    -plane[3]*Lightpos[1];
	SM[0][2] =    -plane[0]*Lightpos[2];
	SM[1][2] =    -plane[1]*Lightpos[2];
	SM[2][2] = dot-plane[2]*Lightpos[2];
	SM[3][2] =    -plane[3]*Lightpos[2];
	SM[0][3] =    -plane[0]*Lightpos[3];
	SM[1][3] =    -plane[1]*Lightpos[3];
	SM[2][3] =    -plane[2]*Lightpos[3];
	SM[3][3] = dot-plane[3]*Lightpos[3];
}
