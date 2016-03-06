#ifndef LINEAR_VECTOR_H
#define LINEAR_VECTOR_H

struct Vector {
	Vector(int x0, int y0, int z0);

	int dot(Vector* other);
	Vector* cross(Vector* other);

	int getMagnitude();
	Vector* unitVector();

	//int ... cos(pheta) = A.B/AB
	//int ... F.U_ab ... gives component of vector F in the direction of vector U

	int x, y, z;
};

#endif