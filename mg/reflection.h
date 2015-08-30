void ReflectionMatrix(float reflection_matrix [4][4], float plane_point[3], float plane_normal[3])
{
	float pv;
	GLfloat *p = (GLfloat*)plane_point,
		    *v = (GLfloat*)plane_normal;
	pv = p[0]*v[0] + p[1]*v[1] + p[2]*v[2];

	reflection_matrix[0][0] = 1- 2 * v[0] * v[0];
	reflection_matrix[1][0] = - 2 * v[0] * v[1];
	reflection_matrix[2][0] = - 2 * v[0] * v[2];
	reflection_matrix[3][0] = 2 * pv * v[0];

	reflection_matrix[0][1] = - 2 * v[0] * v[1];
	reflection_matrix[1][1] = 1- 2 * v[1] * v[1];
	reflection_matrix[2][1] = - 2 * v[1] * v[2];
	reflection_matrix[3][1] = 2 * pv * v[1];

	
	reflection_matrix[0][2] = - 2 * v[0] * v[2];
	reflection_matrix[1][2] = - 2 * v[1] * v[2];
	reflection_matrix[2][2] = 1- 2 * v[2] * v[2];
	reflection_matrix[3][2] = 2 * pv * v[2];

	reflection_matrix[0][3] = 0;
	reflection_matrix[1][3] = 0;
	reflection_matrix[2][3] = 0;
	reflection_matrix[3][3] = 1;
}
