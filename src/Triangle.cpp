#include "Triangle.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <vector>
#include <iostream>

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))

Triangle::Triangle()
{
	v[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	v[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	v[2] = glm::vec3(0.0f, 0.0f, 0.0f);

	c[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	c[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	c[2] = glm::vec3(0.0f, 0.0f, 0.0f);

	t[0] = glm::vec2(0.0f, 0.0f);
	t[1] = glm::vec2(0.0f, 0.0f);
	t[2] = glm::vec2(0.0f, 0.0f);
}

Triangle::Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	c[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	c[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	c[2] = glm::vec3(0.0f, 0.0f, 0.0f);

	t[0] = glm::vec2(0.0f, 0.0f);
	t[1] = glm::vec2(0.0f, 0.0f);
	t[2] = glm::vec2(0.0f, 0.0f);

};

Triangle::Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec2& t0, glm::vec2& t1, glm::vec2& t2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	t[0] = t0;
	t[1] = t1;
	t[2] = t2;

	c[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[2] = glm::vec3(1.0f, 1.0f, 1.0f);

};

void Triangle::setColorCoordinates(glm::vec3 c0, glm::vec3 c1, glm::vec3 c2) {
	c[0] = c0;
	c[1] = c1;
	c[2] = c2;
}

// Rendering the triangle using OpenGL
void Triangle::RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, bool isTextured)
{

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(modelViewMatrix));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));
	
	// For textured object
	if (isTextured)
	{
		glEnable(GL_TEXTURE_2D);

		// Avoid modulating the texture by vertex color
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glBegin(GL_TRIANGLES);

			glTexCoord2f(t[0].x, t[0].y);
			glVertex3f(v[0].x, v[0].y, v[0].z);

			glTexCoord2f(t[1].x, t[1].y);
			glVertex3f(v[1].x, v[1].y, v[1].z);

			glTexCoord2f(t[2].x, t[2].y);
			glVertex3f(v[2].x, v[2].y, v[2].z);

		glEnd();

		glDisable(GL_TEXTURE_2D);


	}
	// For object with only vertex color
	else
	{
		glBegin(GL_TRIANGLES);

			glColor3f(c[0].x, c[0].y, c[0].z);
			glVertex3f(v[0].x, v[0].y, v[0].z);

			glColor3f(c[1].x, c[1].y, c[1].z);
			glVertex3f(v[1].x, v[1].y, v[1].z);

			glColor3f(c[2].x, c[2].y, c[2].z);
			glVertex3f(v[2].x, v[2].y, v[2].z);
		
		glEnd();
	}

}
// double for precision
float triangleArea(float x1, float y1, float x2, float y2, float x3, float y3) {
	float area = (.5) * ((x1 * (y2 - y3) + (x2 * (y3 - y1)) + (x3 * (y1 - y2))));
	return abs(area);
}
bool inside(float x, float y, float x1, float y1, float x2, float y2, float x3, float y3) {
	float totalA = triangleArea(x1,y1,x2,y2,x3,y3);
	float A1 = triangleArea(x,y,x2,y2,x3,y3);
	float A2 = triangleArea(x,y,x1,y1,x3,y3);
	float A3 = triangleArea(x,y,x1,y1,x2,y2);
	if ((A1 + A2 + A3) > totalA) {
		return false;
	} else {
		return true;
	}
}

std::vector<float> calcBary(float x, float y, float x1, float y1, float x2, float y2, float x3, float y3) {

	float alpha = (-1*(x - x2) * (y3 - y2) + (y - y2) * (x3 - x2)) / (-1*(x1 - x2) * (y3 - y2) + (y1 - y2) * (x3 - x2));
	float beta = (-1*(x - x3) * (y1 - y3) + (y - y3) * (x1 - x3)) / (-1*(x2 - x3) * (y1 - y3) + (y2 - y3) * (x1 - x3));
	float gamma = 1 - alpha - beta;
	std::vector<float> bary;
	bary.push_back(alpha);
	bary.push_back(beta);
	bary.push_back(gamma);
	return bary;
}

float wrap_around(float val, int domain) {
	if (val < 0) {
		while(val < 0) {
			val = val + domain;
		}
	}
	if (val > domain) {
		while (val > domain) {
			val = val - domain;
		}
	}
	return val;
}

float lerp(float x, float v0, float v1) {
	float val = v0 + (x * (v1 - v0));
	return val;
}

glm::vec3 bilinear_interp(std::vector<float*> texture, float u_i, float v_i, int texWidth, int texHeight, int depth) {
	u_i *= texWidth;
	v_i *= texHeight;
	u_i -= 0.5;
	v_i -= 0.5;
	int x_l = floor(u_i);
	int x_r = ceil(u_i);
	int y_d = floor(v_i);
	int y_u = ceil(v_i);

	float s = u_i - x_l;
	float t = v_i - y_d;

	x_l = wrap_around(x_l, texWidth - 1);
	x_r = wrap_around(x_r, texWidth - 1);
	y_u = wrap_around(y_u, texHeight - 1);
	y_d = wrap_around(y_d, texHeight - 1);
	
	float u_00r = texture[depth][y_d * texWidth * 3 + x_l * 3 + 0];
	float u_00g = texture[depth][y_d * texWidth * 3 + x_l * 3 + 1];
	float u_00b = texture[depth][y_d * texWidth * 3 + x_l * 3 + 2];

	float u_01r = texture[depth][y_u * texWidth * 3 + x_l * 3 + 0];
	float u_01g = texture[depth][y_u * texWidth * 3 + x_l * 3 + 1];
	float u_01b = texture[depth][y_u * texWidth * 3 + x_l * 3 + 2];

	float u_10r = texture[depth][y_d * texWidth * 3 + x_r * 3 + 0];
	float u_10g = texture[depth][y_d * texWidth * 3 + x_r * 3 + 1];
	float u_10b = texture[depth][y_d * texWidth * 3 + x_r * 3 + 2];

	float u_11r = texture[depth][y_u * texWidth * 3 + x_r * 3 + 0];
	float u_11g = texture[depth][y_u * texWidth * 3 + x_r * 3 + 1];
	float u_11b = texture[depth][y_u * texWidth * 3 + x_r * 3 + 2];

	float u_0r = lerp(s, u_00r, u_10r);
	float u_0g = lerp(s, u_00g, u_10g);
	float u_0b = lerp(s, u_00b, u_10b);

	float u_1r = lerp(s, u_01r, u_11r);
	float u_1g = lerp(s, u_01g, u_11g);
	float u_1b = lerp(s, u_01b, u_11b);

	glm::vec3 retval;
	retval.x = lerp(t, u_0r, u_1r);
	retval.y = lerp(t, u_0g, u_1g);
	retval.z = lerp(t, u_0b, u_1b);
	return retval;
}

// Render the triangle on CPU
void Triangle::RenderCPU(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, float colorBuffer[1024][1024][3], float depth[1024][1024], int MAX_WIDTH, int MAX_HEIGHT,int texWidth, int texHeight, std::vector<float*> texture, bool isTextured, int textureMode)
{
	glm::mat4 viewport = glm::mat4(1.0f);
	viewport[0][0] = ((float)MAX_WIDTH) / 2;
	viewport[1][1] = ((float)MAX_HEIGHT) / 2;
	viewport[3][0] = ((float)MAX_WIDTH) / 2;
	viewport[3][1] = ((float)MAX_WIDTH) / 2;

	glm::vec4 interpol_v0 = modelViewMatrix * glm::vec4(v[0], 1.0f);
	glm::vec4 trans_v0 = projectionMatrix * interpol_v0;
	glm::vec3 temp_v0; 
	temp_v0.x = trans_v0.x / trans_v0.w;
	temp_v0.y = trans_v0.y / trans_v0.w;
	temp_v0.z = trans_v0.z / trans_v0.w;
	glm::vec4 ndc_v0 = viewport * glm::vec4(temp_v0, 1.0f);

	
	glm::vec4 interpol_v1 = modelViewMatrix * glm::vec4(v[1], 1.0f);
	glm::vec4 trans_v1 = projectionMatrix * interpol_v1;
	glm::vec3 temp_v1; 
	temp_v1.x = trans_v1.x / trans_v1.w;
	temp_v1.y = trans_v1.y / trans_v1.w;
	temp_v1.z = trans_v1.z / trans_v1.w;
	glm::vec4 ndc_v1 = viewport * glm::vec4(temp_v1, 1.0f);

	glm::vec4 interpol_v2 = modelViewMatrix * glm::vec4(v[2], 1.0f);
	glm::vec4 trans_v2 = projectionMatrix * interpol_v2;
	glm::vec3 temp_v2; 
	temp_v2.x = trans_v2.x / trans_v2.w;
	temp_v2.y = trans_v2.y / trans_v2.w;
	temp_v2.z = trans_v2.z / trans_v2.w;
	glm::vec4 ndc_v2 = viewport * glm::vec4(temp_v2, 1.0f);



	// create a bounding box from the points
	float xmin, xmax, ymin, ymax;
	float triangle_xs[] = {ndc_v0.x, ndc_v1.x, ndc_v2.x};
	float triangle_ys[] = {ndc_v0.y, ndc_v1.y, ndc_v2.y};
	xmin = triangle_xs[0];
	xmax = triangle_xs[0];
	ymax = triangle_ys[0];
	ymin = triangle_ys[0];
	// find bounds for triangle
	for (int i = 0; i < 3; i++) {
		if (triangle_xs[i] >= xmax) {
			xmax = triangle_xs[i];
		}
		if (triangle_xs[i] <= xmin) {
			xmin = triangle_xs[i];
		}
		if (triangle_ys[i] >= ymax) {
			ymax = triangle_ys[i];
		}
		if (triangle_ys[i] <= ymin) {
			ymin = triangle_ys[i];
		}
	}
	xmin = trunc(xmin);
	ymin = trunc(ymin);
	xmax = ceil(xmax);
	ymax = ceil(ymax);
	std::vector<float> baryCoord;
	glm::vec3 baryV;
	glm::vec3 baryC;
	if (!isTextured) {
		for (int y = ymin; y <= ymax; y++) {
			for (int x = xmin; x <= xmax; x++) {
				baryCoord = calcBary(x+0.5,y+0.5, ndc_v0.x, ndc_v0.y, ndc_v1.x, ndc_v1.y, ndc_v2.x, ndc_v2.y);	
				baryV = (baryCoord[0] * ndc_v0) + (baryCoord[1] * ndc_v1) + (baryCoord[2] * ndc_v2); 
				baryC = (baryCoord[0] * c[0]) + (baryCoord[1] * c[1]) + (baryCoord[2] * c[2]);
				if (baryCoord[0] >= 0 && baryCoord[0] <= 1 && baryCoord[1] >= 0 && baryCoord[1] <= 1 && baryCoord[2] >= 0 && baryCoord[2] <= 1) {
					if (baryV.z < depth[y][x] && x >= 0 && x < MAX_WIDTH && y >=0 && y < MAX_HEIGHT) {
						colorBuffer[y][x][0] = baryC.x;
						colorBuffer[y][x][1] = baryC.y; 
						colorBuffer[y][x][2] = baryC.z;  
						depth[y][x] = baryV.z;
					}
				}
			}
		}
	} else {
		float z_inv_v0 = 1 / interpol_v0.z;
		float z_inv_v1 = 1 / interpol_v1.z;
		float z_inv_v2 = 1 / interpol_v2.z;
		//loop through bounding box pixels
		glm::vec2 q_sca_v0 = t[0] / interpol_v0.z;
		glm::vec2 q_sca_v1 = t[1] / interpol_v1.z;
		glm::vec2 q_sca_v2 = t[2] / interpol_v2.z;

		for (int y = ymin; y <= ymax; y++) {
			for (int x = xmin; x <= xmax; x++) {
				baryCoord = calcBary(x+0.5,y+0.5, ndc_v0.x, ndc_v0.y, ndc_v1.x, ndc_v1.y, ndc_v2.x, ndc_v2.y);	
				baryV = (baryCoord[0] * ndc_v0) + (baryCoord[1] * ndc_v1) + (baryCoord[2] * ndc_v2); 
				float z_inv = (baryCoord[0] * z_inv_v0) + (baryCoord[1] * z_inv_v1) + (baryCoord[2] * z_inv_v2);
				glm::vec2 q_sca = (baryCoord[0] * q_sca_v0) + (baryCoord[1] * q_sca_v1) + (baryCoord[2] * q_sca_v2);
				glm::vec2 final_val = q_sca / z_inv;
				if (textureMode == 0) {
					int lookup_x;
					int lookup_y;
					float u_i;
					float v_i;
					u_i = wrap_around(final_val.x, 1);
					v_i = wrap_around(final_val.y, 1);
					u_i *= texWidth;
					v_i *= texHeight;
					lookup_x = floor(u_i);
					lookup_y = floor(v_i);
					//std::cout << "ui: " << u_i << " vi: " << v_i << std::endl;
					if (baryCoord[0] >= 0 && baryCoord[0] <= 1 && baryCoord[1] >= 0 && baryCoord[1] <= 1 && baryCoord[2] >= 0 && baryCoord[2] <= 1) {
						if (baryV.z < depth[y][x] && x >= 0 && x < MAX_WIDTH && y >=0 && y < MAX_HEIGHT) {
							colorBuffer[y][x][0] = texture[0][lookup_y * texWidth * 3 + lookup_x * 3 + 0];
							colorBuffer[y][x][1] = texture[0][lookup_y * texWidth * 3 + lookup_x * 3 + 1];
							colorBuffer[y][x][2] = texture[0][lookup_y * texWidth * 3 + lookup_x * 3 + 2];
							depth[y][x] = baryV.z;
						}
					}
				}
				if (textureMode == 1) {
					float u_i = final_val.x;
					float v_i = final_val.y;
					glm::vec3 b_interp = bilinear_interp(texture, u_i, v_i, texWidth, texHeight, 0);
					if (baryCoord[0] >= 0 && baryCoord[0] <= 1 && baryCoord[1] >= 0 && baryCoord[1] <= 1 && baryCoord[2] >= 0 && baryCoord[2] <= 1) {
						if (baryV.z < depth[y][x] && x >= 0 && x < MAX_WIDTH && y >=0 && y < MAX_HEIGHT) {
							colorBuffer[y][x][0] = b_interp.x;
							colorBuffer[y][x][1] = b_interp.y;
							colorBuffer[y][x][2] = b_interp.z;
							depth[y][x] = baryV.z;
						}
					}
				}
				if (textureMode == 2) {

					float u_i = final_val.x;
					float v_i = final_val.y;
					u_i *= texWidth;
					v_i *= texHeight;
					

					std::vector<float> baryCoord_x = calcBary(x+1+0.5,y+0.5, ndc_v0.x, ndc_v0.y, ndc_v1.x, ndc_v1.y, ndc_v2.x, ndc_v2.y);	 
					float z_inv_x = (baryCoord_x[0] * z_inv_v0) + (baryCoord_x[1] * z_inv_v1) + (baryCoord_x[2] * z_inv_v2);
					glm::vec2 q_sca_x = (baryCoord_x[0] * q_sca_v0) + (baryCoord_x[1] * q_sca_v1) + (baryCoord_x[2] * q_sca_v2);
					glm::vec2 final_val_x = q_sca_x / z_inv_x;
					
					float u_ix = final_val_x.x * texWidth;
					float v_ix = final_val_x.y * texHeight;

					std::vector<float> baryCoord_y = calcBary(x+0.5,y+1+0.5, ndc_v0.x, ndc_v0.y, ndc_v1.x, ndc_v1.y, ndc_v2.x, ndc_v2.y);	 
					float z_inv_y = (baryCoord_y[0] * z_inv_v0) + (baryCoord_y[1] * z_inv_v1) + (baryCoord_y[2] * z_inv_v2);
					glm::vec2 q_sca_y = (baryCoord_y[0] * q_sca_v0) + (baryCoord_y[1] * q_sca_v1) + (baryCoord_y[2] * q_sca_v2);
					glm::vec2 final_val_y = q_sca_y / z_inv_y;

					float u_iy = final_val_y.x * texWidth;
					float v_iy = final_val_y.y * texHeight;

					float lx = sqrt(pow((u_ix - u_i), 2) + pow((v_ix - v_i), 2));
					float ly = sqrt(pow((u_iy - u_i), 2) + pow((v_iy - v_i), 2));
					float L = fmax(lx, ly);
					float D = log2(L);
					int Db = floor(D);
					int Da = ceil(D);
					
					float d1 = D - Db;
					float d2 = Da - D;
					if (Da > 10) {
						Da = 10;
					}
					if (Db > 10) {
						Db = 10;
					}
					if (Da < 0) {
						Da = 0;
					}
					if (Db < 0) {
						Db = 0;
					}
					
					glm::vec3 ca = bilinear_interp(texture, final_val.x, final_val.y, texWidth, texHeight, Da);

					glm::vec3 cb = bilinear_interp(texture, final_val.x, final_val.y, texWidth, texHeight, Db);
			
					glm::vec3 c = ((d1)*ca) + ((d2)*cb);

					if (baryCoord[0] >= 0 && baryCoord[0] <= 1 && baryCoord[1] >= 0 && baryCoord[1] <= 1 && baryCoord[2] >= 0 && baryCoord[2] <= 1) {
						if (baryV.z < depth[y][x] && x >= 0 && x < MAX_WIDTH && y >=0 && y < MAX_HEIGHT) {
							colorBuffer[y][x][0] = c.x;
							colorBuffer[y][x][1] = c.y;
							colorBuffer[y][x][2] = c.z;
							depth[y][x] = baryV.z;
						}
					}

				}
			}
		}
	}

	
	 
}
