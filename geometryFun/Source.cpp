#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>	

double const r = 1;

struct vertice {
	int accuracy;
	double x, y, z;
	std::vector<vertice*> linkedVertices;
	vertice(double x, double y, double z, std::vector<vertice*> linkedVertices) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->linkedVertices = linkedVertices;
	}
	vertice(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	vertice(double x, double y, double z, int accuracy) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->accuracy = accuracy;
	}
	void printLinks() {
		std::cout << "\n--------Linked vertices--------" << std::endl;
		std::cout << "(" << x << "," << y << "," << z << ") links to: " << std::endl;
		for (int i = 0; i < linkedVertices.size(); i++) {
			std::cout << "(" << linkedVertices[i]->x << "," << linkedVertices[i]->y << "," << linkedVertices[i]->z << ")" << std::endl;
		}
		std::cout << "-------------------------------\n" << std::endl;
	}
};

struct shape {
	double center;
	std::vector<vertice> vertices;
	shape(double center, std::vector<vertice> vertices) {
		this->center = center;
		this->vertices = vertices;
	}
	void printVertices() {
		std::cout << "\n--------Vertices--------" << std::endl;
		for (int i = 0; i < vertices.size(); i++) {
			std::cout << "(" << vertices[i].x << "," << vertices[i].y << "," << vertices[i].z << ")" << std::endl;
		}
		std::cout << "------------------------\n" << std::endl;
	}
};

void octahedronBase(int accuracy = 0);

vertice newVertice(vertice * a, vertice * b, int accuracy);
bool sameOctant(vertice * a, vertice * b);
bool distance(vertice * origin, vertice * destination);

int main() {
	octahedronBase(2);
	system("pause");
	return 0;
}

void octahedronBase(int accuracy) {
	static const vertice holder[6] = { vertice(0,r,0,0), vertice(r,0,0,0), vertice(0,0,r,0), vertice(0,-r,0,0), vertice(-r,0,0,0), vertice(0,0,-r,0) };
	std::vector<vertice> vertices (holder, holder + (sizeof(holder) / sizeof(vertice)));
	shape sphereApproximation = shape(0, vertices);
	int count;
	for (int i = 0; i < 6; i++) {
		count = i;
		for (int t = 0; t < 5; t++) {
			if (count == 5) {
				count = 0;
			}
			else {
				count++;
			}
			if (t == 2) {
				continue;
			}
			sphereApproximation.vertices[i].linkedVertices.push_back(&sphereApproximation.vertices[count]);
		}
		sphereApproximation.vertices[i].printLinks();
	}
	sphereApproximation.printVertices();

	for (int accuracyIterator = 0; accuracyIterator < accuracy; accuracyIterator++){
		bool * newConnection = new bool[pow(sphereApproximation.vertices.size(), 2) - sphereApproximation.vertices.size()]();
		vertice * pointerHolder;
		for (int i = 0; i < sphereApproximation.vertices.size(); i++) {
			std::cout << "i: " << i << std::endl;
			for (int t = 0; t < sphereApproximation.vertices[i].linkedVertices.size(); t++) {
				std::cout << "t: " << t << std::endl;
				if (!newConnection[(i * (sphereApproximation.vertices.size() - 1)) + t]) {
					pointerHolder = sphereApproximation.vertices[i].linkedVertices[t];
					sphereApproximation.vertices.push_back(newVertice(&sphereApproximation.vertices[i], pointerHolder, accuracyIterator + 1));
					
					for (int q = 0; q < pointerHolder->linkedVertices.size(); q++) {
						if (pointerHolder->linkedVertices[q] == &sphereApproximation.vertices[i]) {
							pointerHolder->linkedVertices.erase(pointerHolder->linkedVertices.begin() + q);
							break;
						}
					}
					
					sphereApproximation.vertices[i].linkedVertices.erase(sphereApproximation.vertices[i].linkedVertices.begin() + t);

					sphereApproximation.vertices[i].linkedVertices.push_back(&sphereApproximation.vertices.back());

					std::cout << "gets here" << std::endl;
					pointerHolder->linkedVertices.push_back(&sphereApproximation.vertices.back());
					std::cout << "does not get here" << std::endl;

					sphereApproximation.vertices.back().linkedVertices.push_back(&sphereApproximation.vertices[i]);
					sphereApproximation.vertices.back().linkedVertices.push_back(pointerHolder);
				}
			}
		}
		for (int q = 0; q < sphereApproximation.vertices.size(); q++) {
			for (int p = 0; p < sphereApproximation.vertices.size() - q; p++) {
				if (sphereApproximation.vertices[p].accuracy = (accuracyIterator + 1) && distance(&sphereApproximation.vertices[q], &sphereApproximation.vertices[p]) && sameOctant(&sphereApproximation.vertices[q], &sphereApproximation.vertices[p])) {
					sphereApproximation.vertices[p].linkedVertices.push_back(&sphereApproximation.vertices[q]);
					sphereApproximation.vertices[q].linkedVertices.push_back(&sphereApproximation.vertices[p]);
				}
			}
		}
	}

	sphereApproximation.printVertices();
}

bool distance(vertice * origin, vertice * destination) {
	for (int i = 0; i < origin->linkedVertices.size(); i++) {
		for (int t = 0; t < origin->linkedVertices[i]->linkedVertices.size(); t++) {
			if (origin->linkedVertices[i]->linkedVertices[t] == destination) {
				return true;
			}
		}
	}
	return false;
}

bool sameOctant(vertice * a, vertice * b) {
	if (
		((a->x >= 0 && b->x >= 0) || (a->x <= 0 && b->x <= 0)) && 
		((a->y >= 0 && b->y >= 0) || (a->y <= 0 && b->y <= 0)) && 
		((a->z >= 0 && b->z >= 0) || (a->z <= 0 && b->z <= 0))) {
		return true;
	}
	return false;
}

vertice newVertice(vertice * a, vertice * b, int accuracy) {
	vertice midpoint = vertice((b->x + a->x) / 2, (b->y + a->y) / 2, (b->z + a->z) / 2);

	double midpointMagnitude = sqrt(pow(midpoint.x, 2) + pow(midpoint.y, 2) + pow(midpoint.z, 2));

	midpoint.x /= midpointMagnitude;
	midpoint.y /= midpointMagnitude;
	midpoint.z /= midpointMagnitude;

	return vertice(r * midpoint.x, r * midpoint.y, r * midpoint.z, accuracy);
}