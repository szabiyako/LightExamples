#shader vertex
#version 330 core

out vec2 fragCoord;
void main(void)
{
	vec4 vertices[4] = vec4[4]
	(
		vec4(-1.0, -1.0, 0.0, 1.0),
		vec4(1.0, -1.0, 0.0, 1.0),
		vec4(-1.0, 1.0, 0.0, 1.0),
		vec4(1.0, 1.0, 0.0, 1.0)
	);

	vec2 texCoord[4] = vec2[4]
	(
		vec2(0.0, 0.0),
		vec2(1.0, 0.0),
		vec2(0.0, 1.0),
		vec2(1.0, 1.0)
	);

	fragCoord = texCoord[gl_VertexID];
	gl_Position = vertices[gl_VertexID];
}

#shader fragment
#version 330 core

in vec2 fragCoord;
out vec4 color;

uniform vec3 location;
uniform vec2 screeResolution;
uniform mat3 viewToWorld;
uniform sampler2D texPosition;
uniform sampler2D texNode;
uniform int bvhWidth;
uniform int texPosWidth;

uniform samplerCube u_skybox;
uniform sampler2D u_normalTexture;
uniform float u_fov;

uniform vec3 u_pointLightsPos[32];
uniform int u_nPointLights;
uniform int u_nRaysMax;


//------------------- STRUCT AND LOADER BEGIN -----------------------
struct Triangle
{
    // Position
    vec3 pos1;
    vec3 pos2;
    vec3 pos3;

    // Normal
    vec3 norm1;
    vec3 norm2;
    vec3 norm3;
};
struct Node
{
	int childIsTriangle;
    int leftChild;
    int rightChild;
    vec3 aabbMin;
    vec3 aabbMax;
};

struct Ray
{
    vec3 origin;
    vec3 direction;
    float tStart;
    float tEnd;
};

struct Hit
{
    vec3 normal;
    vec3 planeNormal;
    vec3 position;
    vec2 uv;
    bool isHit;
};


vec2 get2DIndex(int index, int width)
{
	int x = index % width;
    int y = index / width;
    return vec2(x / float(width), y / float(width));
}

Node getNode(int index)
{
	index = index * 3;

	vec3 integerData = texture(texNode, get2DIndex(index, bvhWidth)).rgb;
	vec3 aabbMin = texture(texNode, get2DIndex(index + 1, bvhWidth)).rgb;
	vec3 aabbMax = texture(texNode, get2DIndex(index + 2, bvhWidth)).rgb;

	Node node;
	node.childIsTriangle = int(integerData.x);
	node.leftChild = int(integerData.y);
	node.rightChild = int(integerData.z);
	node.aabbMin = aabbMin;
	node.aabbMax = aabbMax;
	return node;
}

Triangle getTriangle(int index)
{
	index = index * 3;
	Triangle triangle;
	triangle.pos1 = texture(texPosition, get2DIndex(index, texPosWidth)).rgb;
	triangle.pos2 = texture(texPosition, get2DIndex(index + 1, texPosWidth)).rgb;
	triangle.pos3 = texture(texPosition, get2DIndex(index + 2, texPosWidth)).rgb;

	triangle.norm1 = texture(u_normalTexture, get2DIndex(index, texPosWidth)).rgb;
	triangle.norm2 = texture(u_normalTexture, get2DIndex(index + 1, texPosWidth)).rgb;
	triangle.norm3 = texture(u_normalTexture, get2DIndex(index + 2, texPosWidth)).rgb;
	return triangle;
}
//------------------- STRUCT AND LOADER END -----------------------

//------------------- STACK BEGIN -----------------------
int countTI = 0;
int _stack[15];
int _index = -1;

void stackClear()
{
    _index = -1;
}

int stackSize()
{
    return _index + 1;
}

void stackPush(in int node)
{
    if(_index > 14	)
        discard;
    _stack[++_index] = node;
 
}

int stackPop()
{
    return _stack[_index--];
}
//------------------- STACK END -----------------------

bool slabs(in Ray ray, in vec3 minB, in vec3 maxB, inout float localMin) {

    if(all(greaterThan(ray.origin, minB)) && all(lessThan(ray.origin, maxB)))
        return true;

    vec3 t0 = (minB - ray.origin)/ray.direction;
    vec3 t1 = (maxB - ray.origin)/ray.direction;
    vec3 tmin = min(t0, t1), tmax = max(t0, t1);
    float tminf = max(max(tmin.x, tmin.y), tmin.z);
    float tmaxf = min(min(tmax.x, tmax.y), tmax.z);

    if (tminf > tmaxf)
        return false;

    localMin = tminf;
    return tminf < ray.tEnd && tminf > ray.tStart;
}

bool isect_tri(inout Ray ray, in Triangle tri, inout Hit hit ) {
	vec3 e1 = tri.pos2 - tri.pos1;
	vec3 e2 = tri.pos3 - tri.pos1;
	vec3 P = cross(ray.direction, e2);
	float det = dot(e1, P);
	if (abs(det) < 1e-4)
        return false;

	float inv_det = 1. / det;
	vec3 T = (ray.origin - tri.pos1);
	float u = dot(T, P) * inv_det;
	//if (u < 0.0 || u > 1.0)
	//if (u < -0.01 || u > 1.01)
	if (u < -0.000001 || u > 1.000001)
        return false;

	vec3 Q = cross(T, e1);
	float v = dot(ray.direction, Q) * inv_det;
	//if (v < 0.0 || (v+u) > 1.0)
	//if (v < -0.01 || (v+u) > 1.01)
	if (v < -0.000001 || (v+u) > 1.000001)
        return false;

	float tt = dot(e2, Q) * inv_det;

    if(ray.tEnd > tt && ray.tStart < tt )
    {
        vec3 c = vec3(u, v, 1.0 - u - v);
        countTI++;
        hit.normal = (tri.norm1 * c.z + tri.norm2 * c.x + tri.norm3 * c.y);
        hit.planeNormal = normalize(cross(e1, e2));

        hit.position = (ray.origin + ray.direction * tt);
        hit.isHit = true;
        ray.tEnd = tt;
        return true;
    }
    return false;
}

void traceCloseHitV2(inout Ray ray, inout Hit hit)
{
    stackClear();
    stackPush(0);
    hit.isHit = false;
    Node select;
    Triangle try;
    float tempt;

    while(stackSize() != 0)
    {
        select = getNode(stackPop());
        if(!slabs(ray, select.aabbMin, select.aabbMax, tempt))
            continue;
        
        if(select.childIsTriangle == 0)
        {
            float leftMinT = 0;
            float rightMinT= 0;
            Node right = getNode(select.rightChild);
            Node left = getNode(select.leftChild);
            bool rightI = slabs(ray, right.aabbMin, right.aabbMax,  rightMinT);
            bool leftI = slabs(ray, left.aabbMin, left.aabbMax,  leftMinT);

            if(rightI && leftI)
            {
                if (rightMinT < leftMinT)
                {
                    stackPush(select.leftChild);
                    stackPush(select.rightChild);
                }
                else
                {
                    stackPush(select.rightChild);
                    stackPush(select.leftChild);
                }
                continue;
            }
            if(rightI)
                stackPush(select.rightChild);
            else
                stackPush(select.leftChild);
            continue;
        }

        if ((select.childIsTriangle & 2) == 0)
            stackPush(select.rightChild);

        if ((select.childIsTriangle & 1) == 0)
            stackPush(select.leftChild);

        if((select.childIsTriangle & 2) > 0)
        {
            try = getTriangle(select.rightChild);
            isect_tri(ray, try, hit);
        }

        if((select.childIsTriangle & 1) > 0)
        {
            try = getTriangle(select.leftChild);
            isect_tri(ray, try, hit);
        }
    }
}

mat3 rotationMatrix(vec3 axis, float angle)
{
   axis = normalize(axis);
   float s = sin(angle);
   float c = cos(angle);
   float oc = 1.0 - c;

   return mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
               oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
               oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c);
}

void main() {
    vec3 viewDir = normalize(vec3((gl_FragCoord.xy * 2 - screeResolution.xy) * tan(radians(u_fov)/2) / screeResolution.y, -1.0));
    vec3 worldDir = viewToWorld * viewDir;

    int nRays = u_nRaysMax;

    //const float offset = 0.0001;
    const float offset = 0.001;
    const vec3 baseColor = vec3(1, 1, 1);
    //const vec3 lightPos = vec3(-50, 100, 5);

    float currentColorPower = 0.9;
    vec3 currentPos = location;
    vec3 currentDir = worldDir;

    ///////////////////////////////////////
    
    vec3 resultColor = vec3(0, 0, 0);
    int rayIndex = 0;
    for (rayIndex = 0; rayIndex < nRays; ++rayIndex) {
        Ray ray;
        ray.origin = currentPos;
        ray.direction = currentDir;
        ray.tStart = 0.0001;
        ray.tEnd = 10000;

        Hit hit;
        traceCloseHitV2(ray, hit);

        if (!hit.isHit) {
            vec3 skyBoxPos = vec3(ray.direction.x, -ray.direction.y, ray.direction.z);
            if (rayIndex == 0) {
                color = texture(u_skybox, skyBoxPos);
                return;
            }
            else {
                resultColor += currentColorPower * vec3(texture(u_skybox, skyBoxPos));
                break;
            }
        }
        currentPos = hit.position + hit.planeNormal * offset;
        currentDir = reflect(ray.direction, hit.normal);

        if (nRays == 1) {
            resultColor = hit.planeNormal;
            break;
        }
        else if (nRays == 2) {
            resultColor = hit.normal;
            break;
        }
        //Light rays
        for (int lightIndex = 0; lightIndex < u_nPointLights; ++lightIndex) {
            vec3 lightPos = u_pointLightsPos[lightIndex];

            Ray lightRay;
            lightRay.direction = normalize(lightPos - hit.position);
            lightRay.origin = currentPos;
            lightRay.tStart = 0.000001;
            lightRay.tEnd = length(lightPos - hit.position);

            float dotProduct = dot(hit.normal, lightRay.direction);
            float colorKoeff = (dotProduct + 1.0) / 2.0;
            //resultColor += currentColorPower * colorKoeff * baseColor;
            //currentColorPower *= currentColorPower;

            //if (dotProduct < 0)
            //    continue;

            Hit lightHit;
            traceCloseHitV2(lightRay, lightHit);

            if (lightHit.isHit) {
                //resultColor +=  currentColorPower * (1 - colorKoeff) * baseColor;
                //resultColor -=  currentColorPower * colorKoeff * baseColor;
                resultColor = baseColor * 0.1;
            }
            else {
                //resultColor += currentColorPower * colorKoeff * baseColor;
                //resultColor += baseColor;
                resultColor = baseColor;
            }
            //resultColor = hit.planeNormal;
            //resultColor += currentColorPower * colorKoeff * baseColor;
            //if (lightHit.isHit)
            //    if (dotProduct < 0)
            //        resultColor -= 0.05 * baseColor;
            //    else
            //        resultColor -= 0.2 * baseColor;

            currentColorPower *= currentColorPower;
        }
        if (nRays > 2)
            break;
    }
    //color = vec4(resultColor/(rayIndex + 1), 1);
    color = vec4(resultColor/(rayIndex + 1), 1);
}
