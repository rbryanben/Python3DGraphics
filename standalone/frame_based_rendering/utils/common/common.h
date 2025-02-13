#ifndef common_h
#define common_h
#include <vector>
#include <math.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <strstream>
#include "uuid.h"
#include "math.h"

using namespace std;

// Predefine structures
struct Mesh;
struct Triangle;
struct Matrix4x4;

Matrix4x4 getScalingMatrix(float f);
Matrix4x4 getMatrixRotationX(float rotationAngle);
Matrix4x4 getMatrixRotationY(float rotationAngle);
Matrix4x4 getMatrixRotationZ(float rotationAngle);

// Precalculated
float pi = 22.f / 7.f ;

// Round to 2 decimal places
float round_(float var)
{
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}

// Vect3d 
struct Vect3d{
    float x,y,z;
    float w = 1.0f;
    // Overload subtration
    Vect3d operator - (Vect3d other){
        Vect3d res;
        res.x = this->x - other.x;
        res.y = this->y - other.y;
        res.z = this->z - other.z;

        return res;
    }  

    Vect3d operator + (Vect3d other){
        Vect3d res;
        res.x = this->x + other.x;
        res.y = this->y + other.y;
        res.z = this->z + other.z;

        return res;
    }  

    Vect3d operator * (float multiplier){
        return {this->x * multiplier, this->y * multiplier, this->z * multiplier, 1};
    }

    bool operator == (Vect3d other){
        if (round_(x) == round_(other.x) && round_(y) == round_(other.y) && round_(z) == round_(other.z)){
            return true;
        }
        return false;
    }
};

// Structure to hold 2 lists of projected and original points 
struct TrianglesHolder {
    vector<Triangle> original; 
    vector<Triangle> projected; 
    TrianglesHolder(){};
    TrianglesHolder(vector<Triangle> projected, vector<Triangle> original){
        this->original = original;
        this->projected = projected;
    }
};

// Vect2d
struct Vect2d{
    float u,v;
    float w = 1 ;

};

// Resolution class 
struct Resolution{
    int width,height; 
};

// Color 
struct Color
{
    float red = 0.0f,green=0.0f,blue = 0.0f;
};

// Triangle 
struct Triangle
{
    Vect3d p[3];
    Vect2d t[3];
    Color color ;
    Image* texture;
    int textureNumber;
    //Mesh* parent;

    // This is used in shadow mapping, we need to refere back to the original MatViewdPoint 

    bool operator ==  (Triangle &other){
        if (other.p[0] == p[0]
            && other.p[1] == p[1]
            && other.p[2] == p[2]){
                return true;
            }

        return false;
    }

    void copyTextureFrom(Triangle &triangle){
        this->t[0] = triangle.t[0];
        this->t[1] = triangle.t[1];
        this->t[2] = triangle.t[2];
        // Copy texture reference
        this->texture = triangle.texture;
        // Copy color as well 
        this->color = triangle.color;
    }

};

// 4x4 Matrix
struct Matrix4x4{
    float m[4][4]= {0.0f};

    double getHash(){
        return 
            m[0][0] + m[0][1] + m[0][2] + m[0][3] +
            m[1][0] + m[1][1] + m[1][2] + m[1][3] + 
            m[2][0] + m[2][1] + m[2][2] + m[2][3] + 
            m[3][0] + m[3][1] + m[3][2] + m[3][3];  
    }

    void setTranslation(Vect3d translation){
        m[3][0] = translation.x;
        m[3][1] = translation.y;
        m[3][2] = translation.z;
    }

    Matrix4x4 operator * (Matrix4x4 b){
        Matrix4x4 res; 

        res.m[0][0] = b.m[3][0]*m[0][3]+m[0][0]*b.m[0][0]+m[0][1]*b.m[1][0]+m[0][2]*b.m[2][0]; 
        res.m[0][1] = b.m[3][1]*m[0][3]+m[0][0]*b.m[0][1]+m[0][1]*b.m[1][1]+m[0][2]*b.m[2][1]; 
        res.m[0][2] = b.m[2][2]*m[0][2]+b.m[3][2]*m[0][3]+m[0][0]*b.m[0][2]+m[0][1]*b.m[1][2]; 
        res.m[0][3] = b.m[2][3]*m[0][2]+b.m[3][3]*m[0][3]+m[0][0]*b.m[0][3]+m[0][1]*b.m[1][3]; 


        res.m[1][0] = b.m[3][0]*m[1][3]+m[1][1]*b.m[1][0]+m[1][2]*b.m[2][0]+b.m[0][0]*m[1][0];
        res.m[1][1] = b.m[3][1]*m[1][3]+m[1][1]*b.m[1][1]+m[1][2]*b.m[2][1]+b.m[0][1]*m[1][0];
        res.m[1][2] = b.m[2][2]*m[1][2]+b.m[3][2]*m[1][3]+m[1][1]*b.m[1][2]+b.m[0][2]*m[1][0];
        res.m[1][3] = b.m[2][3]*m[1][2]+b.m[3][3]*m[1][3]+m[1][1]*b.m[1][3]+b.m[0][3]*m[1][0];


        res.m[2][0] = b.m[3][0]*m[2][3]+m[2][0]*b.m[0][0]+m[2][1]*b.m[1][0]+m[2][2]*b.m[2][0]; 
        res.m[2][1] = b.m[3][1]*m[2][3]+m[2][0]*b.m[0][1]+m[2][1]*b.m[1][1]+m[2][2]*b.m[2][1]; 
        res.m[2][2] = b.m[2][2]*m[2][2]+b.m[3][2]*m[2][3]+m[2][0]*b.m[0][2]+m[2][1]*b.m[1][2]; 
        res.m[2][3] = b.m[2][3]*m[2][2]+b.m[3][3]*m[2][3]+m[2][0]*b.m[0][3]+m[2][1]*b.m[1][3];
        
        res.m[3][0] = b.m[3][0]*m[3][3]+m[3][0]*b.m[0][0]+m[3][1]*b.m[1][0]+m[3][2]*b.m[2][0]; 
        res.m[3][1] = b.m[3][1]*m[3][3]+m[3][0]*b.m[0][1]+m[3][1]*b.m[1][1]+m[3][2]*b.m[2][1]; 
        res.m[3][2] = b.m[2][2]*m[3][2]+b.m[3][2]*m[3][3]+m[3][0]*b.m[0][2]+m[3][1]*b.m[1][2]; 
        res.m[3][3] = b.m[2][3]*m[3][2]+b.m[3][3]*m[3][3]+m[3][0]*b.m[0][3]+m[3][1]*b.m[1][3];

        return res ;
    } 


};



// Mesh Structure 
struct Mesh
{
    // List of a meshes triangles
    vector<Triangle> triangles;
    // Mesh Texture
    Image texture;
    // Mesh Geometric Matrix
    Matrix4x4 geometryMatrix;

    // Name 
    string name;

    // loads from file 
	bool LoadFromObjectFile(string sFilename, Image texture=NULL)
	{
        //set texture
        this->texture = texture;

		ifstream f(sFilename);
		if (!f.is_open())
			return false;

		// Local cache of verts
		vector<Vect3d> verts;
		vector<Vect2d> texs;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			strstream s;
			s << line;

			char junk;

			if (line[0] == 'v')
			{
				if (line[1] == 't')
				{
					Vect2d  v;
					s >> junk >> junk >> v.u >> v.v;
					// A little hack for the spyro texture
					//v.u = 1.0f - v.u;
					//v.v = 1.0f - v.v;
					texs.push_back(v);
				}
				else
				{
					Vect3d v;
					s >> junk >> v.x >> v.y >> v.z;
					verts.push_back(v);
				}
			}

			if (!this->texture.hasData())
			{
				if (line[0] == 'f')
				{
					int f[3];
					s >> junk >> f[0] >> f[1] >> f[2];
					triangles.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
				}
			}
			else
			{
				if (line[0] == 'f')
				{
					s >> junk;

					string tokens[6];
					int nTokenCount = -1;


					while (!s.eof())
					{
						char c = s.get();
						if (c == ' ' || c == '/')
							nTokenCount++;
						else
							tokens[nTokenCount].append(1, c);
					}

					tokens[nTokenCount].pop_back();

                    // Triangle to add to the list
                    Triangle tri = { verts[stoi(tokens[0]) - 1], verts[stoi(tokens[2]) - 1], verts[stoi(tokens[4]) - 1],
						texs[stoi(tokens[1]) - 1], texs[stoi(tokens[3]) - 1], texs[stoi(tokens[5]) - 1] };
                    // Set texture reference
                    tri.texture = &this->texture;

                    //tri.parent = this;
					triangles.push_back(tri);

				}

			}
		}
		return true;
	}
    //initialize 
    void init(){
        geometryMatrix.m[0][0] = 1.f;
        geometryMatrix.m[1][1] = 1.f;
        geometryMatrix.m[2][2] = 1.f;
        geometryMatrix.m[3][3] = 1.f;

    }
    // Constructors
    Mesh(){
        name = uuid::generate_uuid_v4();
        init();
    }

    Mesh(string name){
        this->name = name;
        init();
    }

    Mesh(string name,Vect3d translation_vector){
        init();
        this->translate(translation_vector);
    }

    Mesh (string name, Matrix4x4 geometryMatrix){
        init();
        this->geometryMatrix = geometryMatrix;
    }

    // Sets the translation for matrix
    void translate(Vect3d translation_vector){
        geometryMatrix.m[3][0] = translation_vector.x;
        geometryMatrix.m[3][1] = -translation_vector.y;
        geometryMatrix.m[3][2] = translation_vector.z;
    }

    // Rotate z 
    void rotateZ(float rotationAngle){
        geometryMatrix = geometryMatrix * getMatrixRotationZ(rotationAngle * (pi/180));
    }

    void rotateY(float rotationAngle){
        geometryMatrix = geometryMatrix * getMatrixRotationY(rotationAngle * (pi/180));
    }

    void rotateX(float rotationAngle){
        geometryMatrix = geometryMatrix * getMatrixRotationX(rotationAngle * (pi/180));
    }

    // Scale 
    void scale(float scale_factor){
        geometryMatrix.m[0][0] *= scale_factor;
        geometryMatrix.m[1][1] *= scale_factor;
        geometryMatrix.m[2][2] *= scale_factor;
    }

    // Rotate x 
    void rotate(float degrees){

    }
};

// Max 
float f_max(float a,float b){
    if (a > b) return a ;
    return b;
}

float f_min(float a, float b){
    if (a < b) return a;
    return b;
}

// Cross Producut
Vect3d VectorCrossProduct(Vect3d line1, Vect3d line2){
    Vect3d res;
    res.x = (line1.y * line2.z) - (line1.z * line2.y);
    res.y = (line1.z * line2.x) - (line1.x * line2.z);
    res.z = (line1.x * line2.y) - (line1.y * line2.x);

    return res;
}

// Vector Length
float VectorLength(Vect3d vector){
    return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

// Normalize Vector (To Screen Space)
void NormalizeVector(Vect3d &vector){
    float l = VectorLength(vector);
    vector.x /= l;
    vector.y /= l;
    vector.z /= l;
}

Vect3d VectorNormalize(Vect3d &v1){
    float l = VectorLength(v1);
    if (l != 0){
        return {v1.x / l , v1.y / l, v1.z / l, 1};
    }
    return {v1.x,v1.y,v1.z,1};
}

// Vector Dot Product
float VectorDotProduct(Vect3d a,Vect3d b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Vector Multiply (For Scaling)
void VectorMultiplyFloat(Vect3d &vector,float f){
    vector.x *= f ;
    vector.y *= f ;
    vector.z *= f ;
}


// Vector multiply float return new vector
Vect3d VectorMultiplyFloat_Return(Vect3d &vector,float f){
    Vect3d res;
    res.x *= f ;
    res.y *= f ;
    res.z *= f ;

    return res;
}


// Vector Devide (For Scaling)
void VectorDevideFloat(Vect3d &vector,float f){
    vector.x /= f ;
    vector.y /= f ;
    vector.z /= f ;
}


// Triangle Depth
float getTriangleDepth(Triangle &triangle){
    return (triangle.p[0].z + triangle.p[1].z + triangle.p[2].z) / 3 ;
}

// Sort Triangle List
vector<Triangle> sortTriangleList(vector<Triangle> triangleList){
    if (triangleList.size() <= 1) return triangleList;
    
    // Pop back as pivot 
    Triangle pivot = triangleList.back();
    triangleList.pop_back();

    // Lesser and greater
    vector<Triangle> lesser,greater;

    //find lesser and greater
    while (triangleList.size() > 0){
        // Pop back
        Triangle tri = triangleList.back();
        triangleList.pop_back();

        //check if it is greater(deeper)
        if (getTriangleDepth(tri) > getTriangleDepth(pivot)){
            greater.push_back(tri);
        }
        else {
            lesser.push_back(tri);
        }
    }

    //sort greater
    greater = sortTriangleList(greater);
    lesser = sortTriangleList(lesser);

    //Join the lists
    greater.push_back(pivot);
    greater.insert(greater.end(),lesser.begin(),lesser.end());
    return greater;
}

// Calculates where a vector intersects a plane
Vect3d vectorIntersectPlane(Vect3d plane_p, Vect3d plane_n, Vect3d lineStart,Vect3d lineEnd,float &t){
    plane_n = VectorNormalize(plane_n);
    float plane_d = -VectorDotProduct(plane_n,plane_p);
    float ad = VectorDotProduct(lineStart,plane_n);
    float bd = VectorDotProduct(lineEnd,plane_n);
    t = (-plane_d - ad) / (bd - ad);
    Vect3d lineStartToEnd = lineEnd - lineStart;
    Vect3d lineIntersect = lineStartToEnd * t ;
    return lineStart + lineIntersect;
}

// Tirangle clip against plane 
int Triangle_ClipAgainstPlane(Vect3d plane_p,Vect3d plane_n,Triangle &in_tri,Triangle &out_tri1,Triangle &out_tri2){
    //make sure normal is indeed normal
    plane_n = VectorNormalize(plane_n);

    //Returns shortest distance from point to a plane
    auto dist = [&](Vect3d &p){
        Vect3d n = VectorNormalize(p);
        return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - VectorDotProduct(plane_n,plane_p));
    };

    //Create two temporary storage arrays to classify points either side of the plane
    //if the distance is positibe, point lies inside the plane
    vector<Vect3d> inside_points;
    vector<Vect3d> outside_points;

    // get signed distance from each point in triangle to the plane
    float d0 = dist(in_tri.p[0]);
    float d1 = dist(in_tri.p[1]);
    float d2 = dist(in_tri.p[2]);

    if (d0 >= 0) inside_points.push_back(in_tri.p[0]);
    else outside_points.push_back(in_tri.p[0]);
    if (d1 >= 0) inside_points.push_back(in_tri.p[1]);
    else outside_points.push_back(in_tri.p[1]);
    if (d2 >= 0) inside_points.push_back(in_tri.p[2]);
    else outside_points.push_back(in_tri.p[2]); 

    if (inside_points.size() == 0) return 0; //dont draw any

    if (inside_points.size() == 3){ 
        out_tri1.color = in_tri.color;
        out_tri1.p[0] = in_tri.p[0];
        out_tri1.p[1] = in_tri.p[1];
        out_tri1.p[2] = in_tri.p[2];
        return 1;
    }

    // 2 points are outside
    if (inside_points.size() == 1 && outside_points.size() == 2){
        float t1,t2;
        out_tri1.color = {0,0,1};

        //inside point is valid we keep that 
        out_tri1.p[0] = inside_points[0];

        //but the two new points are at the locations where the 
	    //original sides of the triangle (lines) intersect with the plane
        out_tri1.p[1] = vectorIntersectPlane(plane_p,plane_n,inside_points[0],outside_points[0],t1);
        out_tri1.p[2] = vectorIntersectPlane(plane_p,plane_n,inside_points[0],outside_points[1],t2);
        return 1;
    }
    
    if (inside_points.size() == 2 && outside_points.size() == 1){
        float t1,t2;
        out_tri2.color = in_tri.color;
        out_tri1.color = in_tri.color;

        //The first triangle consists of the two inside points and a new
		//point determined by the location where one side of the triangle
		//intersects with the plane
        out_tri1.p[0] = inside_points[0];
        out_tri1.p[1] = inside_points[1];
        out_tri1.p[2] = vectorIntersectPlane(plane_p,plane_n,inside_points[1],outside_points[0],t1);

        out_tri2.p[0] = inside_points[0];
        out_tri2.p[1] = vectorIntersectPlane(plane_p,plane_n,inside_points[1],outside_points[0],t2);
        out_tri2.p[2] = vectorIntersectPlane(plane_p,plane_n,inside_points[0],outside_points[0],t2);

        out_tri1.color = {1,0,0};
        out_tri2.color = {0,1,0};
        return 2;
    }
}


TrianglesHolder clipTriangleAgainstPlane(Vect3d plane_p,Vect3d plane_n,Triangle &in_tri, Triangle &og_in_tri);

TrianglesHolder clipTriangleAgainstPlane(Vect3d plane_p,Vect3d plane_n,vector<Triangle> triangles,vector<Triangle> og_triangles){
    vector<Triangle> res_proj;
    vector<Triangle> res_og;

    //loop through all triangles and clip, then add the result to the final res
    for (int i = 0 ; i != triangles.size(); i++){
        TrianglesHolder clipped = clipTriangleAgainstPlane(plane_p,plane_n,triangles[i],og_triangles[i]);
        // Add triangle to res (Not optimum)
        //for (int j =0; j != clipped.projected.size(); j++){
        //    //res_proj.push_back(clipped.projected[j]);
        //    res_og.push_back(clipped.original[j]);
        //}
        res_proj.insert(res_proj.end(), clipped.projected.begin(), clipped.projected.end());
        res_og.insert(res_og.end(),clipped.original.begin(),clipped.original.end());
    }

    TrianglesHolder res; 
    res.original = res_og;
    res.projected = res_proj;
    return res;
} 

TrianglesHolder clipTriangleAgainstPlane(Vect3d plane_p,Vect3d plane_n,Triangle &in_tri, Triangle &og_in_tri){
    // Result
    TrianglesHolder res_holder; 
    
    // Result vector - a vector of triangle vectors where index 0 will contained clipped of projected triangles and 1 will contain
    //      clipped of original triangles  
    vector<Triangle> res_proj;
    vector<Triangle> res_og;

    // Out tri's 
    Triangle out_tri1,out_tri2;
    Triangle og_out_tri1, og_out_tri2; 

    // Totongogara ta coper ma texture 
    //     og pretty much doesnt care about texture 
    out_tri1.texture = in_tri.texture;
    out_tri2.texture = in_tri.texture;

    //Normalize plane normal
    plane_n = VectorNormalize(plane_n);

    //Returns shortest distance from point to a plane
    auto dist = [&](Vect3d &p){
        Vect3d n = VectorNormalize(p);
        return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - VectorDotProduct(plane_n,plane_p));
    };

    //Create two temporary storage arrays to classify points either side of the plane
    //if the distance is positibe, point lies inside the plane
    vector<Vect3d> inside_points;
    vector<Vect3d> og_inside_points; 
    vector<Vect3d> outside_points;
    vector<Vect3d> og_outside_points;
    vector<Vect2d> inside_texture;
    vector<Vect2d> outside_texture;

    // get signed distance from each point in triangle to the plane
    float d0 = dist(in_tri.p[0]);
    float d1 = dist(in_tri.p[1]);
    float d2 = dist(in_tri.p[2]);

    if (d0 >= 0){
        inside_points.push_back(in_tri.p[0]);
        og_inside_points.push_back(og_in_tri.p[0]);
        inside_texture.push_back(in_tri.t[0]);
    }
    else {
        outside_points.push_back(in_tri.p[0]);
        og_outside_points.push_back(og_in_tri.p[0]);
        outside_texture.push_back(in_tri.t[0]);
    }
    if (d1 >= 0){
        inside_points.push_back(in_tri.p[1]);
        og_inside_points.push_back(og_in_tri.p[1]);
        inside_texture.push_back(in_tri.t[1]);
    }
    else {
        outside_points.push_back(in_tri.p[1]);
        og_outside_points.push_back(og_in_tri.p[1]);
        outside_texture.push_back(in_tri.t[1]);
    } 
    if (d2 >= 0) {
        inside_points.push_back(in_tri.p[2]);
        og_inside_points.push_back(og_in_tri.p[2]);
        inside_texture.push_back(in_tri.t[2]);
    }
    else { 
        outside_points.push_back(in_tri.p[2]); 
        og_outside_points.push_back(og_in_tri.p[2]);
        outside_texture.push_back(in_tri.t[2]);
    }

    if (inside_points.size() == 0) return res_holder; //dont draw any

    // All points are inside 
    if (inside_points.size() == 3){ 
        // Add the triangle to res 0 
        res_proj.push_back(in_tri);
        res_og.push_back(og_in_tri);
        // Add the 2 lists to the final list
        res_holder.projected = res_proj;
        res_holder.original = res_og;
        return res_holder;
    }

    // 2 points are outside - form one new triangle 
    if (inside_points.size() == 1 && outside_points.size() == 2){
        float t1,t2,t3 = 0.f;
        // color blue 
        out_tri1.color = {0,0,1};

        //inside point is valid we keep that 
        out_tri1.p[0] = inside_points[0];
        og_out_tri1.p[0] = og_inside_points[0];

        //but the two new points are at the locations where the 
	    //original sides of the triangle (lines) intersect with the plane
        out_tri1.p[1] = vectorIntersectPlane(plane_p,plane_n,inside_points[0],outside_points[0],t1);
        out_tri1.p[2] = vectorIntersectPlane(plane_p,plane_n,inside_points[0],outside_points[1],t2);

        // Original points
        og_out_tri1.p[1].x = og_inside_points[0].x + (t1 * (og_outside_points[0].x - og_inside_points[0].x));
        og_out_tri1.p[1].y = og_inside_points[0].y + (t1 * (og_outside_points[0].y - og_inside_points[0].y));
        og_out_tri1.p[1].z = og_inside_points[0].z + (t1 * (og_outside_points[0].z - og_inside_points[0].z));
        
        og_out_tri1.p[2].x = og_inside_points[0].x + (t2 * (og_outside_points[1].x - og_inside_points[0].x));
        og_out_tri1.p[2].y = og_inside_points[0].y + (t2 * (og_outside_points[1].y - og_inside_points[0].y));
        og_out_tri1.p[2].z = og_inside_points[0].z + (t2 * (og_outside_points[1].z - og_inside_points[0].z));

        // Add texture 
        out_tri1.t[0] = inside_texture[0];
        out_tri1.t[1].u = inside_texture[0].u + (t1 * (outside_texture[0].u - inside_texture[0].u));
        out_tri1.t[1].v = inside_texture[0].v + (t1 * (outside_texture[0].v - inside_texture[0].v));
        out_tri1.t[1].w = inside_texture[0].w + (t1 * (outside_texture[0].w - inside_texture[0].w));
        out_tri1.t[2].u = inside_texture[0].u + (t2 * (outside_texture[1].u - inside_texture[0].u));
        out_tri1.t[2].v = inside_texture[0].v + (t2 * (outside_texture[1].v - inside_texture[0].v));
        out_tri1.t[2].w = inside_texture[0].w + (t2 * (outside_texture[1].w - inside_texture[0].w));
        
        // Create lists
        res_proj.push_back(out_tri1);
        res_og.push_back(og_out_tri1);
        
        // Add to result holder
        res_holder.projected = res_proj;
        res_holder.original = res_og;

        return res_holder;
    }

    // One outside point - form two new triangles 
    if (inside_points.size() == 2 && outside_points.size() == 1){
        float t1,t2,t3;

        //The first triangle consists of the two inside points and a new
		//point determined by the location where one side of the triangle
		//intersects with the plane
        out_tri1.p[0] = inside_points[0];
        og_out_tri1.p[0] = og_inside_points[0];
        
        out_tri1.p[1] = inside_points[1];
        og_out_tri1.p[1] = og_inside_points[1];

        out_tri1.p[2] = vectorIntersectPlane(plane_p,plane_n,inside_points[1],outside_points[0],t1);
        og_out_tri1.p[2].x = og_inside_points[1].x + (t1 * (og_outside_points[0].x - og_inside_points[1].x));
        og_out_tri1.p[2].y = og_inside_points[1].y + (t1 * (og_outside_points[0].y - og_inside_points[1].y));
        og_out_tri1.p[2].z = og_inside_points[1].z + (t1 * (og_outside_points[0].z - og_inside_points[1].z));

        out_tri1.t[0] = inside_texture[0];
        out_tri1.t[1] = inside_texture[1];
        out_tri1.t[2].u = inside_texture[1].u + (t1 * (outside_texture[0].u - inside_texture[1].u));
        out_tri1.t[2].v = inside_texture[1].v + (t1 * (outside_texture[0].v - inside_texture[1].v));
        out_tri1.t[2].w = inside_texture[1].w + (t1 * (outside_texture[0].w - inside_texture[1].w));

        //form the other triangle
        out_tri2.p[0] = inside_points[0];
        og_out_tri2.p[0] = og_inside_points[0];
        out_tri2.t[0] = inside_texture[0];

        out_tri2.p[1] = vectorIntersectPlane(plane_p,plane_n,inside_points[1],outside_points[0],t2);

        og_out_tri2.p[1].x = og_inside_points[1].x + (t2 * (og_outside_points[0].x - og_inside_points[1].x));
        og_out_tri2.p[1].y = og_inside_points[1].y + (t2 * (og_outside_points[0].y - og_inside_points[1].y));
        og_out_tri2.p[1].z = og_inside_points[1].z + (t2 * (og_outside_points[0].z - og_inside_points[1].z));
        
        out_tri2.t[1].u = inside_texture[1].u + (t2 * (outside_texture[0].u - inside_texture[1].u));
        out_tri2.t[1].v = inside_texture[1].v + (t2 * (outside_texture[0].v - inside_texture[1].v));
        out_tri2.t[1].w = inside_texture[1].w + (t2 * (outside_texture[0].w - inside_texture[1].w));
        
        out_tri2.p[2] = vectorIntersectPlane(plane_p,plane_n,inside_points[0],outside_points[0],t3);
        
        og_out_tri2.p[2].x = og_inside_points[0].x + (t3 * (og_outside_points[0].x - og_inside_points[0].x));
        og_out_tri2.p[2].y = og_inside_points[0].y + (t3 * (og_outside_points[0].y - og_inside_points[0].y));
        og_out_tri2.p[2].z = og_inside_points[0].z + (t3 * (og_outside_points[0].z - og_inside_points[0].z));

        out_tri2.t[2].u = inside_texture[0].u + (t3 * (outside_texture[0].u - inside_texture[0].u));
        out_tri2.t[2].v = inside_texture[0].v + (t3 * (outside_texture[0].v - inside_texture[0].v));
        out_tri2.t[2].w = inside_texture[0].w + (t3 * (outside_texture[0].w - inside_texture[0].w));

        out_tri1.color = {1,0,0};
        out_tri2.color = {0,1,0};

        res_proj.push_back(out_tri1);
        res_proj.push_back(out_tri2);
        res_og.push_back(og_out_tri1);
        res_og.push_back(og_out_tri2);

        res_holder.projected = res_proj;
        res_holder.original = res_og;
        return res_holder;
    } 

}

vector<Triangle> clipTriangleAgainstPlane(Vect3d plane_p,Vect3d plane_n,Triangle &in_tri){
    vector<Triangle> res; 
    Triangle out_tri1,out_tri2;

    // Totongogara ta coper ma texture
    out_tri1.texture = in_tri.texture;
    out_tri2.texture = in_tri.texture;

    //make sure normal is indeed normal
    plane_n = VectorNormalize(plane_n);
    

    //Returns shortest distance from point to a plane
    auto dist = [&](Vect3d &p){
        Vect3d n = VectorNormalize(p);
        return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - VectorDotProduct(plane_n,plane_p));
    };

    //Create two temporary storage arrays to classify points either side of the plane
    //if the distance is positibe, point lies inside the plane
    vector<Vect3d> inside_points;
    vector<Vect3d> outside_points;
    vector<Vect2d> inside_texture;
    vector<Vect2d> outside_texture;

    // get signed distance from each point in triangle to the plane
    float d0 = dist(in_tri.p[0]);
    float d1 = dist(in_tri.p[1]);
    float d2 = dist(in_tri.p[2]);

    if (d0 >= 0){
        inside_points.push_back(in_tri.p[0]);
        inside_texture.push_back(in_tri.t[0]);
    }
    else {
        outside_points.push_back(in_tri.p[0]);
        outside_texture.push_back(in_tri.t[0]);
    }
    if (d1 >= 0){
        inside_points.push_back(in_tri.p[1]);
        inside_texture.push_back(in_tri.t[1]);
    }
    else {
        outside_points.push_back(in_tri.p[1]);
        outside_texture.push_back(in_tri.t[1]);
    } 
    if (d2 >= 0) {
        inside_points.push_back(in_tri.p[2]);
        inside_texture.push_back(in_tri.t[2]);
    }
    else { 
        outside_points.push_back(in_tri.p[2]); 
        outside_texture.push_back(in_tri.t[2]);
    }

    if (inside_points.size() == 0) return res; //dont draw any

    // All points are inside 
    if (inside_points.size() == 3){ 

        res.push_back(in_tri);
        return res;
    }

    // 2 points are outside - form one new triangle 
    if (inside_points.size() == 1 && outside_points.size() == 2){
        float t1,t2,t3 = 0.f;
        // color blue 
        out_tri1.color = {0,0,1};

        //inside point is valid we keep that 
        out_tri1.p[0] = inside_points[0];

        //but the two new points are at the locations where the 
	    //original sides of the triangle (lines) intersect with the plane
        out_tri1.p[1] = vectorIntersectPlane(plane_p,plane_n,inside_points[0],outside_points[0],t1);
        out_tri1.p[2] = vectorIntersectPlane(plane_p,plane_n,inside_points[0],outside_points[1],t2);

        // Add texture 
        out_tri1.t[0] = inside_texture[0];
        out_tri1.t[1].u = inside_texture[0].u + (t1 * (outside_texture[0].u - inside_texture[0].u));
        out_tri1.t[1].v = inside_texture[0].v + (t1 * (outside_texture[0].v - inside_texture[0].v));
        out_tri1.t[1].w = inside_texture[0].w + (t1 * (outside_texture[0].w - inside_texture[0].w));
        out_tri1.t[2].u = inside_texture[0].u + (t2 * (outside_texture[1].u - inside_texture[0].u));
        out_tri1.t[2].v = inside_texture[0].v + (t2 * (outside_texture[1].v - inside_texture[0].v));
        out_tri1.t[2].w = inside_texture[0].w + (t2 * (outside_texture[1].w - inside_texture[0].w));
        
        res.push_back(out_tri1);
        return res;
    }
    
    // One outside point - form two new triangles 
    if (inside_points.size() == 2 && outside_points.size() == 1){
        float t1,t2,t3;

        //The first triangle consists of the two inside points and a new
		//point determined by the location where one side of the triangle
		//intersects with the plane
        out_tri1.p[0] = inside_points[0];
        out_tri1.p[1] = inside_points[1];
        out_tri1.p[2] = vectorIntersectPlane(plane_p,plane_n,inside_points[1],outside_points[0],t1);
        
        out_tri1.t[0] = inside_texture[0];
        out_tri1.t[1] = inside_texture[1];
        out_tri1.t[2].u = inside_texture[1].u + (t1 * (outside_texture[0].u - inside_texture[1].u));
        out_tri1.t[2].v = inside_texture[1].v + (t1 * (outside_texture[0].v - inside_texture[1].v));
        out_tri1.t[2].w = inside_texture[1].w + (t1 * (outside_texture[0].w - inside_texture[1].w));

        //form the other triangle
        out_tri2.p[0] = inside_points[0];
        out_tri2.t[0] = inside_texture[0];
        out_tri2.p[1] = vectorIntersectPlane(plane_p,plane_n,inside_points[1],outside_points[0],t2);
        
        out_tri2.t[1].u = inside_texture[1].u + (t2 * (outside_texture[0].u - inside_texture[1].u));
        out_tri2.t[1].v = inside_texture[1].v + (t2 * (outside_texture[0].v - inside_texture[1].v));
        out_tri2.t[1].w = inside_texture[1].w + (t2 * (outside_texture[0].w - inside_texture[1].w));
        
        out_tri2.p[2] = vectorIntersectPlane(plane_p,plane_n,inside_points[0],outside_points[0],t3);

        out_tri2.t[2].u = inside_texture[0].u + (t3 * (outside_texture[0].u - inside_texture[0].u));
        out_tri2.t[2].v = inside_texture[0].v + (t3 * (outside_texture[0].v - inside_texture[0].v));
        out_tri2.t[2].w = inside_texture[0].w + (t3 * (outside_texture[0].w - inside_texture[0].w));

        out_tri1.color = {1,0,0};
        out_tri2.color = {0,1,0};
        res.push_back(out_tri1);
        res.push_back(out_tri2);
        return res;
    }

}


// Returns normal to a triangle 
Vect3d getTriangleNormal(Triangle &tri){
    Vect3d normal,line1,line2;
    line1 = tri.p[1] - tri.p[0];
    line2 = tri.p[2] - tri.p[0]; 
    normal = VectorCrossProduct(line1,line2);
    NormalizeVector(normal);
    return normal;
}

// Calculates if floating points are equal
bool areEqual(float a, float b, float threshold) {
    if (abs(a - b) <= threshold) return true;
    return false;
}

#endif
