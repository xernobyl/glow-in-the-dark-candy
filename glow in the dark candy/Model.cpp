#include "Model.h"
#include "Window.h"
#include <vector>


Model::~Model()
{
	//glDeleteBuffers(1, &vbo);
}


void BestFitNormal(const Vector3 &v, unsigned char *c)
{
	/*float ev = 9999.9999f;
	unsigned char ei;

	float m = maxElem(absPerElem(v));

	for(unsigned i = 0; i < 128; ++i)
	{
		const float s = 2.0f / 255.0f;
		const float b = -1.0f;

		unsigned char x = i;
		unsigned char y = i;
		unsigned char z = i;

		Vector3 t = normalize(Vector3(s * x + b, s * y + b, s * z + b));
		float e = dot(t, v);

		if(e <= ev)
		{
			ev = e;
			ei = i;
		}
	}*/
}


bool Model::Load(const char* filepath)
{
	std::vector<std::vector<unsigned short>> vneighbor;

	union
	{
		void* v;
		unsigned char* u8;
		unsigned short* u16;
		unsigned* u32;
		float* f32;
	} filepointer;

	int filelength;

	void* file = Window::LoadFile(filepath, &filelength);
	if(!file)
		return false;

	filepointer.v = file;

	unsigned char flags = *filepointer.u8;
	++filepointer.u8;
	unsigned short nvertices = *filepointer.u16;
	++filepointer.u16;

	float scalex, scaley, scalez, biasx, biasy, biasz;
	if(!(flags & F_FLOAT))
	{
		scalex = *filepointer.f32;
		++filepointer.f32;
		scaley = *filepointer.f32;
		++filepointer.f32;
		scalez = *filepointer.f32;
		++filepointer.f32;
		biasx = *filepointer.f32;
		++filepointer.f32;
		biasy = *filepointer.f32;
		++filepointer.f32;
		biasz = *filepointer.f32;
		++filepointer.f32;
	}

	unsigned nstrips = 1;
	if(flags & F_STRIPS)
	{
		nstrips = *filepointer.u16;
		++filepointer.u16;
	}

	std::vector<float> vertex;
	vertex.reserve(nvertices * 3);
	vneighbor.resize(nvertices);

	if((flags & F_WEIGHTS) == 0)
	{	
		if(flags & F_FLOAT)
		{
			vertex.assign(filepointer.f32, filepointer.f32 + nvertices * 3);
			filepointer.f32 += nvertices * 3;
		}
		else
		{
			for(unsigned i = 0; i < nvertices; ++i)
			{
				float x = scalex * *filepointer.u16 + biasx;
				++filepointer.u16;
				float y = scaley * *filepointer.u16 + biasy;
				++filepointer.u16;
				float z = scalez * *filepointer.u16 + biasz;
				++filepointer.u16;

				vertex.push_back(x);
				vertex.push_back(y);
				vertex.push_back(z);
			}
		}
	}

	strip.resize(nstrips);

	for(unsigned i = 0; i < nstrips; ++i)
	{
		unsigned n = *filepointer.u16;
		++filepointer.u16;

		strip[i].resize(n);

		for(unsigned j = 0; j < n; ++j)
		{
			strip[i][j].uv[0] = -1;
			strip[i][j].uv[1] = -1;
			strip[i][j].uv[2] = -1;
			strip[i][j].uv[3] = -1;
			strip[i][j].uv[4] = -1;
			strip[i][j].uv[5] = -1;

			if(flags & F_HASUVS)
			{
				strip[i][j].vertex[0] = *filepointer.u16;
				++filepointer.u16;
				strip[i][j].uv[0] = *filepointer.u16;
				++filepointer.u16;
				strip[i][j].uv[1] = *filepointer.u16;
				++filepointer.u16;
				strip[i][j].vertex[1] = *filepointer.u16;
				++filepointer.u16;
				strip[i][j].uv[2] = *filepointer.u16;
				++filepointer.u16;
				strip[i][j].uv[3] = *filepointer.u16;
				++filepointer.u16;
				strip[i][j].vertex[2] = *filepointer.u16;
				++filepointer.u16;
				strip[i][j].uv[4] = *filepointer.u16;
				++filepointer.u16;
				strip[i][j].uv[5] = *filepointer.u16;
				++filepointer.u16;
			}
			else
			{
				strip[i][j].vertex[0] = *filepointer.u16;
				++filepointer.u16;
				strip[i][j].vertex[1] = *filepointer.u16;
				++filepointer.u16;
				strip[i][j].vertex[2] = *filepointer.u16;
				++filepointer.u16;
			}

			vneighbor[strip[i][j].vertex[0]].push_back(i);
			vneighbor[strip[i][j].vertex[0]].push_back(j);

			vneighbor[strip[i][j].vertex[1]].push_back(i);
			vneighbor[strip[i][j].vertex[1]].push_back(j);

			vneighbor[strip[i][j].vertex[2]].push_back(i);
			vneighbor[strip[i][j].vertex[2]].push_back(j);

			float q1[3], q2[3];

			q1[0] = vertex[strip[i][j].vertex[1] * 3 + 0] - vertex[strip[i][j].vertex[0] * 3 + 0];
			q1[1] = vertex[strip[i][j].vertex[1] * 3 + 1] - vertex[strip[i][j].vertex[0] * 3 + 1];
			q1[2] = vertex[strip[i][j].vertex[1] * 3 + 2] - vertex[strip[i][j].vertex[0] * 3 + 2];

			q2[0] = vertex[strip[i][j].vertex[2] * 3 + 0] - vertex[strip[i][j].vertex[0] * 3 + 0];
			q2[1] = vertex[strip[i][j].vertex[2] * 3 + 1] - vertex[strip[i][j].vertex[0] * 3 + 1];
			q2[2] = vertex[strip[i][j].vertex[2] * 3 + 2] - vertex[strip[i][j].vertex[0] * 3 + 2];

			strip[i][j].normal[0] = q1[1] * q2[2] - q1[2] * q2[1];
			strip[i][j].normal[1] = q1[2] * q2[0] - q1[0] * q2[2];
			strip[i][j].normal[2] = q1[0] * q2[1] - q1[1] * q2[0];

			float l = sqrt(strip[i][j].normal[0] * strip[i][j].normal[0] +
				strip[i][j].normal[1] * strip[i][j].normal[1] +
				strip[i][j].normal[2] * strip[i][j].normal[2]);

			strip[i][j].normal[0] /= l;
			strip[i][j].normal[1] /= l;
			strip[i][j].normal[2] /= l;

			if(flags & F_HASUVS)
			{				
				float u1 = (float)(strip[i][j].uv[2] - strip[i][j].uv[0]);// / 65535.0f;
				float v1 = (float)(strip[i][j].uv[3] - strip[i][j].uv[1]);// / 65535.0f;
				float u2 = (float)(strip[i][j].uv[4] - strip[i][j].uv[0]);// / 65535.0f;
				float v2 = (float)(strip[i][j].uv[5] - strip[i][j].uv[1]);// / 65535.0f;

				//float inv = 1.0f / (u1 * v2 - u2 * v1);

				strip[i][j].tangent[0] = (v2 * q1[0] - v1 * q2[0]);
				strip[i][j].tangent[1] = (v2 * q1[1] - v1 * q2[1]);
				strip[i][j].tangent[2] = (v2 * q1[2] - v1 * q2[2]);

				l = sqrt(strip[i][j].tangent[0] * strip[i][j].tangent[0] +
					strip[i][j].tangent[1] * strip[i][j].tangent[1] +
					strip[i][j].tangent[2] * strip[i][j].tangent[2]);

				strip[i][j].tangent[0] /= l;
				strip[i][j].tangent[1] /= l;
				strip[i][j].tangent[2] /= l;

				strip[i][j].bitangent[0] = (u1 * q2[0] - u2 * q1[0]);
				strip[i][j].bitangent[1] = (u1 * q2[1] - u2 * q1[1]);
				strip[i][j].bitangent[2] = (u1 * q2[2] - u2 * q1[2]);

				l = sqrt(strip[i][j].bitangent[0] * strip[i][j].bitangent[0] +
					strip[i][j].bitangent[1] * strip[i][j].bitangent[1] +
					strip[i][j].bitangent[2] * strip[i][j].bitangent[2]);

				strip[i][j].bitangent[0] /= l;
				strip[i][j].bitangent[1] /= l;
				strip[i][j].bitangent[2] /= l;
			}
		}
	}

	delete [] file;

	std::vector<float> buffer;
	buffer.reserve(vertex.size() * sizeof(float) * 6);

	for(unsigned i = 0; i < strip.size(); ++i)
	{
		for(unsigned j = 0; j < strip[i].size(); ++j)
		{
			float normal0[3], normal1[3], normal2[3];
			float tangent0[3], tangent1[3], tangent2[3];
			float bitangent0[3], bitangent1[3], bitangent2[3];

			normal2[0] = normal1[0] = normal0[0] = 0.0;
			normal2[1] = normal1[1] = normal0[1] = 0.0;
			normal2[2] = normal1[2] = normal0[2] = 0.0;

			if(flags & F_HASUVS)
			{
				tangent2[0] = tangent1[0] = tangent0[0] = 0.0;
				tangent2[1] = tangent1[1] = tangent0[1] = 0.0;
				tangent2[2] = tangent1[2] = tangent0[2] = 0.0;

				bitangent2[0] = bitangent1[0] = bitangent0[0] = 0.0;
				bitangent2[1] = bitangent1[1] = bitangent0[1] = 0.0;
				bitangent2[2] = bitangent1[2] = bitangent0[2] = 0.0;
			}

			for(unsigned k = 0; k < vneighbor[strip[i][j].vertex[0]].size() / 2; ++k)
				if(vneighbor[strip[i][j].vertex[0]][k * 2 + 0] == i)
				{
					normal0[0] += strip[i][vneighbor[strip[i][j].vertex[0]][k * 2 + 1]].normal[0];
					normal0[1] += strip[i][vneighbor[strip[i][j].vertex[0]][k * 2 + 1]].normal[1];
					normal0[2] += strip[i][vneighbor[strip[i][j].vertex[0]][k * 2 + 1]].normal[2];

					if(flags & F_HASUVS)
					{
						tangent0[0] += strip[i][vneighbor[strip[i][j].vertex[0]][k * 2 + 1]].tangent[0];
						tangent0[1] += strip[i][vneighbor[strip[i][j].vertex[0]][k * 2 + 1]].tangent[1];
						tangent0[2] += strip[i][vneighbor[strip[i][j].vertex[0]][k * 2 + 1]].tangent[2];

						bitangent0[0] += strip[i][vneighbor[strip[i][j].vertex[0]][k * 2 + 1]].bitangent[0];
						bitangent0[1] += strip[i][vneighbor[strip[i][j].vertex[0]][k * 2 + 1]].bitangent[1];
						bitangent0[2] += strip[i][vneighbor[strip[i][j].vertex[0]][k * 2 + 1]].bitangent[2];
					}
				}

			for(unsigned k = 0; k < vneighbor[strip[i][j].vertex[1]].size() / 2; ++k)
				if(vneighbor[strip[i][j].vertex[1]][k * 2 + 0] == i)
				{
					normal1[0] += strip[i][vneighbor[strip[i][j].vertex[1]][k * 2 + 1]].normal[0];
					normal1[1] += strip[i][vneighbor[strip[i][j].vertex[1]][k * 2 + 1]].normal[1];
					normal1[2] += strip[i][vneighbor[strip[i][j].vertex[1]][k * 2 + 1]].normal[2];

					if(flags & F_HASUVS)
					{
						tangent1[0] += strip[i][vneighbor[strip[i][j].vertex[1]][k * 2 + 1]].tangent[0];
						tangent1[1] += strip[i][vneighbor[strip[i][j].vertex[1]][k * 2 + 1]].tangent[1];
						tangent1[2] += strip[i][vneighbor[strip[i][j].vertex[1]][k * 2 + 1]].tangent[2];

						bitangent1[0] += strip[i][vneighbor[strip[i][j].vertex[1]][k * 2 + 1]].bitangent[0];
						bitangent1[1] += strip[i][vneighbor[strip[i][j].vertex[1]][k * 2 + 1]].bitangent[1];
						bitangent1[2] += strip[i][vneighbor[strip[i][j].vertex[1]][k * 2 + 1]].bitangent[2];
					}
				}

			for(unsigned k = 0; k < vneighbor[strip[i][j].vertex[2]].size() / 2; ++k)
				if(vneighbor[strip[i][j].vertex[2]][k * 2 + 0] == i)
				{
					normal2[0] += strip[i][vneighbor[strip[i][j].vertex[2]][k * 2 + 1]].normal[0];
					normal2[1] += strip[i][vneighbor[strip[i][j].vertex[2]][k * 2 + 1]].normal[1];
					normal2[2] += strip[i][vneighbor[strip[i][j].vertex[2]][k * 2 + 1]].normal[2];

					if(flags & F_HASUVS)
					{
						tangent2[0] += strip[i][vneighbor[strip[i][j].vertex[2]][k * 2 + 1]].tangent[0];
						tangent2[1] += strip[i][vneighbor[strip[i][j].vertex[2]][k * 2 + 1]].tangent[1];
						tangent2[2] += strip[i][vneighbor[strip[i][j].vertex[2]][k * 2 + 1]].tangent[2];

						bitangent2[0] += strip[i][vneighbor[strip[i][j].vertex[2]][k * 2 + 1]].bitangent[0];
						bitangent2[1] += strip[i][vneighbor[strip[i][j].vertex[2]][k * 2 + 1]].bitangent[1];
						bitangent2[2] += strip[i][vneighbor[strip[i][j].vertex[2]][k * 2 + 1]].bitangent[2];
					}
				}

			buffer.push_back(vertex[strip[i][j].vertex[0] * 3 + 0]);
			buffer.push_back(vertex[strip[i][j].vertex[0] * 3 + 1]);
			buffer.push_back(vertex[strip[i][j].vertex[0] * 3 + 2]);

			if(flags & F_HASUVS)
			{
				buffer.push_back((float)strip[i][j].uv[0] / 65535.0f);
				buffer.push_back((float)strip[i][j].uv[1] / 65535.0f);
			}

			buffer.push_back(normal0[0]);
			buffer.push_back(normal0[1]);
			buffer.push_back(normal0[2]);

			if(flags & F_HASUVS)
			{
				buffer.push_back(bitangent0[0]);
				buffer.push_back(bitangent0[1]);
				buffer.push_back(bitangent0[2]);

				buffer.push_back(tangent0[0]);
				buffer.push_back(tangent0[1]);
				buffer.push_back(tangent0[2]);
			}

			buffer.push_back(vertex[strip[i][j].vertex[1] * 3 + 0]);
			buffer.push_back(vertex[strip[i][j].vertex[1] * 3 + 1]);
			buffer.push_back(vertex[strip[i][j].vertex[1] * 3 + 2]);

			if(flags & F_HASUVS)
			{
				buffer.push_back((float)strip[i][j].uv[2] / 65535.0f);
				buffer.push_back((float)strip[i][j].uv[3] / 65535.0f);
			}

			buffer.push_back(normal1[0]);
			buffer.push_back(normal1[1]);
			buffer.push_back(normal1[2]);

			if(flags & F_HASUVS)
			{
				buffer.push_back(bitangent1[0]);
				buffer.push_back(bitangent1[1]);
				buffer.push_back(bitangent1[2]);

				buffer.push_back(tangent1[0]);
				buffer.push_back(tangent1[1]);
				buffer.push_back(tangent1[2]);
			}

			buffer.push_back(vertex[strip[i][j].vertex[2] * 3 + 0]);
			buffer.push_back(vertex[strip[i][j].vertex[2] * 3 + 1]);
			buffer.push_back(vertex[strip[i][j].vertex[2] * 3 + 2]);

			if(flags & F_HASUVS)
			{
				buffer.push_back((float)strip[i][j].uv[4] / 65535.0f);
				buffer.push_back((float)strip[i][j].uv[5] / 65535.0f);
			}

			buffer.push_back(normal2[0]);
			buffer.push_back(normal2[1]);
			buffer.push_back(normal2[2]);

			if(flags & F_HASUVS)
			{
				buffer.push_back(bitangent2[0]);
				buffer.push_back(bitangent2[1]);
				buffer.push_back(bitangent2[2]);

				buffer.push_back(tangent2[0]);
				buffer.push_back(tangent2[1]);
				buffer.push_back(tangent2[2]);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	if(flags & F_HASUVS)
	{
		ntriangles = buffer.size() / 42;

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer.size(), buffer.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*14, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*14, (void*)(sizeof(float)*3));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*14, (void*)(sizeof(float)*5));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float)*14, (void*)(sizeof(float)*8));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(float)*14, (void*)(sizeof(float)*11));
		glEnableVertexAttribArray(4);
	}
	else
	{
		ntriangles = buffer.size() / 18;
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer.size(), buffer.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(sizeof(float)*3));
		glEnableVertexAttribArray(2);
	}

	return true;
}


void Model::Draw()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawArrays(GL_TRIANGLES, 0, ntriangles * 3);
}
