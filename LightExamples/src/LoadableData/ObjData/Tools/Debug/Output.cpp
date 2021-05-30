#include "Output.h"

#include "LoadableData/ObjData/ObjData.h"

template <class Vector>
void printVector(std::ostream& output, std::vector<Vector> vector, const int nDims)
{
	output << '[';
	const size_t size = vector.size();
	for (size_t i = 0; i < size; ++i) {
		const Vector& vec = vector[i];
		output << '{';
		for (int d = 0; d < nDims; ++d) {
			output << vec[d] << ", ";
		}
		output << "\b\b}, ";
	}
	if (size == 0)
		output << ']' << std::endl;
	else
		output << "\b\b]" << std::endl;
}

template <class T>
void print(std::ostream& output, std::vector<T> vector)
{
	output << '[';
	const size_t size = vector.size();
	for (size_t i = 0; i < size; ++i) {
		output << vector[i] << ", ";
	}
	if (size == 0)
		output << ']' << std::endl;
	else
		output << "\b\b]" << std::endl;
}

std::ostream& operator<<(std::ostream& output, const ObjData& objData)
{
	output << "ObjData Debug info:" << std::endl;
	output << "-------------------" << std::endl;
	output << "Data:" << std::endl;
	output << "vertices: ";
	printVector(output, objData.data.vertices, 3);
	output << "normals: ";
	printVector(output, objData.data.normals, 3);
	output << "uv: ";
	printVector(output, objData.data.uv, 2);

	output << "Indices:" << std::endl;
	output << "polygonsStarts: ";
	print(output, objData.indices.polygonsStarts);
	output << "polygons: ";
	print(output, objData.indices.polygons);
	output << "normals: ";
	print(output, objData.indices.normals);
	output << "uv: ";
	print(output, objData.indices.uv);
	
	return output;
}
