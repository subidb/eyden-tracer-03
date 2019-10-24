#include "Scene.h"
#include <fstream>
#include "ShaderEyelight.h"
#include "ShaderPhong.h"
#include "PrimTriangle.h"

using namespace std;

void CScene::ParseOBJ(const string& fileName)
{
	cout << "Parsing OBJFile : " << fileName << endl;

	shared_ptr<IShader> pShader = make_shared<CShaderEyelight>(RGB(1, 1, 1));
	// --- PUT YOUR CODE HERE ---
	vector<unsigned int> node_idx;
	vector<Vec3f> temp_vertices;

	ifstream file;
	file.open(fileName);
	if (!file.is_open())
	{
		cout << "Error" << endl;
		return;
	}

	while(!file.eof())
	{
		char line = file.get();
		if (line == 'v')
		{
			Vec3f vertex;
			float a, b ,c;
			file >> a >> b >> c;
			vertex[0] = a * 99.99f;
			vertex[1] = b * 99.99f;
			vertex[2] = c * 99.99f;
			temp_vertices.push_back(vertex);
		}

		else if(line == 'f')
		{
			unsigned int vertexIndex[3];
			unsigned int a, b, c;
			file >>a >> b >> c;
			vertexIndex[0] = a;
			vertexIndex[1] = b;
			vertexIndex[2] = c;
			node_idx.push_back(vertexIndex[0]);
			node_idx.push_back(vertexIndex[1]);
			node_idx.push_back(vertexIndex[2]);
		}
	}

	for(int i = 0;i < node_idx.size();)
	{
		Vec3f v1,v2,v3;
		v1 = temp_vertices[node_idx[i] - 1];
		i++;
		v2 = temp_vertices[node_idx[i] - 1];
		i++;
		v3 = temp_vertices[node_idx[i] - 1];
		i++;
		Add(make_shared<CPrimTriangle>(v1, v2, v3,pShader));
	}
	file.close();


	cout << "Finished Parsing" << endl;
	return;
}

