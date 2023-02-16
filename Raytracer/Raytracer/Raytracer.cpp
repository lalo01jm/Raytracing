#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

#define eps 0.0001

//vector en R^3
class vec {
public:
	double x, y, z;
	vec() {
		x = 0;
		y = 0;
		z = 0;
	}
	vec(double x, double y, double z) {
		x = x;
		y = y;
		z = z;
	}
	double norm() {
		return(sqrt(x * x + y * y + z * z));
	}
	//vector mas constante
	vec operator/(double c) {
		return vec(x / c, y / c, z / c);
	}
	vec operator*(double c) {
		return vec(x * c, y * c, z * c);
	}
	vec operator-(double c) {
		return vec(x - c, y - c, z - c);
	}
	vec operator+(double c) {
		return vec(x + c, y + c, z + c);
	}
	//vector mas vector
	vec operator/(vec v) {
		return vec(x / v.x, y / v.y, z / v.z);
	}
	vec operator*(vec v) {
		return vec(x * v.x, y * v.y, z * v.z);
	}
	vec operator-(vec v) {
		return vec(x - v.x, y - v.y, z - v.z);
	}
	vec operator+(vec v) {
		return vec(x + v.x, y + v.y, z + v.z);
	}
};

class color {
public:
	int r, g, b;
	color() {
		r = 0;
		g = 0;
		b = 0;
	}
	color(int red, int green, int blue) {
		r = red;
		g = green;
		b = blue;
	}
};

double dot(vec a, vec b) {
	vec c = a * b;
	return c.x + c.y + c.z;
}

class Rayo {//rayo con dirección normalizada
public:
	vec origen, dir;
	Rayo(vec o, vec d) {
		origen = o;
		dir = d / d.norm();
	}
};

//primitiva esfera
class esfera {
public:
	vec centro;
	double radio;
	color col;
	esfera(vec c, double r, color co) {
		centro = c;
		radio = r;
		col = co;
	}
	bool interseccion(Rayo r, double& t) {//t es la constante de la interseccion r.origen+t*r.dir
		vec o = r.origen;
		vec d = r.dir;
		vec oc = o - centro;
		double b = 2 * dot(oc, d);
		double c = dot(oc, oc) - radio * radio;
		double disc = b * b - 4 * c;
		if (disc < eps) {
			return false;
		}
		disc = sqrt(disc);
		const double t0 = -b - disc;
		const double t1 = -b + disc;
		t = (t0 < t1) ? t0 : t1;
		return true;
		//std::cout << t << std::endl;
		//double t1 = (radio * radio + 2.0 * dot(r.origen, r.dir) - dot(r.origen, r.origen)) / dot(r.dir, r.dir);
		//std::cout << t1 << std::endl;
		//if (t < eps)
		//	return false;
		//t1 = sqrt(t1);
		//double t2 = -t1;
		//if ((r.dir * t1).norm() < (r.dir * t2).norm())
		//	t = t1;
		//else
		//	t = t2;
		//return true;
	}
};

//primitiva cubo
class cubo {
public:
	vec esquinaizq;
	vec esquinader;
	cubo(vec i, vec d) {
		esquinaizq = i;
		esquinader = d;
	}
};

int main()
{
	//dimensiones de la imagen
	const int ancho = 500, alto = 500;

	std::ofstream out("out.ppm");
	out << "P3\n" << ancho << '\n' << alto << '\n' << "255\n";

	color rojo(255, 0, 0);
	color verde(0, 255, 0);
	color azul(0, 0, 255);
	color blanco(255, 255, 255);

	color** pantalla = new color * [alto];
	for (int i = 0; i < alto; i++) {
		pantalla[i] = new color[ancho];
		for (int j = 0; j < ancho; j++) {
			pantalla[i][j] = blanco;
		}
	}

	esfera S(vec(ancho * 0.5, alto * 0.5, 50), 50, rojo);

	//lanzar los rayos
	for (int j = 0; j < ancho; j++) {
		for (int i = 0; i < alto; i++) {
			Rayo ray(vec(i, j, 0), vec(0, 0, 1));
			//Rayo ray(vec(ancho * 0.5, alto * 0.5, -500), vec(i, j, 0));

			double t = 10000;

			if (S.interseccion(ray, t))
				pantalla[i][j] = azul;
			//std::cout << t << std::endl;
			out << (int)pantalla[i][j].r << ' ';
			out << (int)pantalla[i][j].g << ' ';
			out << (int)pantalla[i][j].b << '\n';
		}
	}

	for (int i = 0; i < alto; i++) {
		delete pantalla[i];
	}
	delete pantalla;

	return 0;
}