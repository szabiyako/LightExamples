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
layout(location = 0) out vec4 color;

uniform vec3 location;
uniform vec2 screenResolution;
uniform mat3 viewToWorld;
uniform sampler2D texPosition;
uniform sampler2D texNode;
uniform int bvhWidth;
uniform int texPosWidth;

uniform samplerCube u_skybox;
uniform sampler2D u_normalTexture;
uniform sampler2D u_previousTexture;
uniform float u_fov;

uniform vec3 u_pointLightsPos[32];
uniform float u_pointLightsScale[32];
uniform int u_nPointLights;
uniform int u_nRaysMax;
uniform float u_samplePart;

uniform vec2 u_seed1;
uniform vec2 u_seed2;

uniform vec2 u_SamplePerPixelOffset;

uvec4 R_STATE;

const float PI = 3.14159265359;

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
const int _stackSize = 20;
const int _stackLast = _stackSize - 1;
int countTI = 0;
int _stack[_stackSize];
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
    if (_index > _stackLast)
        discard;
    _stack[++_index] = node;

}

int stackPop()
{
    return _stack[_index--];
}
//------------------- STACK END -----------------------

bool slabs(in Ray ray, in vec3 minB, in vec3 maxB, inout float localMin) {

    if (all(greaterThan(ray.origin, minB)) && all(lessThan(ray.origin, maxB)))
        return true;

    vec3 t0 = (minB - ray.origin) / ray.direction;
    vec3 t1 = (maxB - ray.origin) / ray.direction;
    vec3 tmin = min(t0, t1), tmax = max(t0, t1);
    float tminf = max(max(tmin.x, tmin.y), tmin.z);
    float tmaxf = min(min(tmax.x, tmax.y), tmax.z);

    if (tminf > tmaxf)
        return false;

    localMin = tminf;
    return tminf < ray.tEnd&& tminf > ray.tStart;
}

bool isect_tri(inout Ray ray, in Triangle tri, inout Hit hit) {
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
    if (v < -0.000001 || (v + u) > 1.000001)
        return false;

    float tt = dot(e2, Q) * inv_det;

    if (ray.tEnd > tt && ray.tStart < tt)
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

    while (stackSize() != 0)
    {
        select = getNode(stackPop());
        if (!slabs(ray, select.aabbMin, select.aabbMax, tempt))
            continue;

        if (select.childIsTriangle == 0)
        {
            float leftMinT = 0;
            float rightMinT = 0;
            Node right = getNode(select.rightChild);
            Node left = getNode(select.leftChild);
            bool rightI = slabs(ray, right.aabbMin, right.aabbMax, rightMinT);
            bool leftI = slabs(ray, left.aabbMin, left.aabbMax, leftMinT);

            if (rightI && leftI)
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
            if (rightI)
                stackPush(select.rightChild);
            else
                stackPush(select.leftChild);
            continue;
        }

        if ((select.childIsTriangle & 2) == 0)
            stackPush(select.rightChild);

        if ((select.childIsTriangle & 1) == 0)
            stackPush(select.leftChild);

        if ((select.childIsTriangle & 2) > 0)
        {
            try = getTriangle(select.rightChild);
            isect_tri(ray, try, hit);
        }

        if ((select.childIsTriangle & 1) > 0)
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

    return mat3(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s,
        oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s,
        oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c);
}

uint TausStep(uint z, int S1, int S2, int S3, uint M)
{
    uint b = (((z << S1) ^ z) >> S2);
    return (((z & M) << S3) ^ b);
}

uint LCGStep(uint z, uint A, uint C)
{
    return (A * z + C);
}

vec2 hash22(vec2 p)
{
    p += u_seed1.x;
    vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xx + p3.yz) * p3.zy);
}

float random()
{
    R_STATE.x = TausStep(R_STATE.x, 13, 19, 12, uint(4294967294));
    R_STATE.y = TausStep(R_STATE.y, 2, 25, 4, uint(4294967288));
    R_STATE.z = TausStep(R_STATE.z, 3, 11, 17, uint(4294967280));
    R_STATE.w = LCGStep(R_STATE.w, uint(1664525), uint(1013904223));
    return 2.3283064365387e-10 * float((R_STATE.x ^ R_STATE.y ^ R_STATE.z ^ R_STATE.w));
}

vec3 randomOnSphere() {
    vec3 rand = vec3(random(), random(), random());
    float theta = rand.x * 2.0 * 3.14159265;
    float v = rand.y;
    float phi = acos(2.0 * v - 1.0);
    float r = pow(rand.z, 1.0 / 3.0);
    float x = r * sin(phi) * cos(theta);
    float y = r * sin(phi) * sin(theta);
    float z = r * cos(phi);
    return vec3(x, y, z);
}
/*
bool intersectLightSrc(Ray ray, vec3 lightPos, float radius)
{
    return false;
    float cx = lightPos.x;
    float cy = lightPos.y;
    float cz = lightPos.z;

    float px = ray.origin.x * 1000;
    float py = ray.origin.y * 1000;
    float pz = ray.origin.z * 1000;

    float vx = ray.direction.x * ray.tEnd;
    float vy = ray.direction.y * ray.tEnd;
    float vz = ray.direction.z * ray.tEnd;

    float A = vx * vx + vy * vy + vz * vz;
    float B = 2.0 * (px * vx + py * vy + pz * vz - vx * cx - vy * cy - vz * cz);
    float C = px * px - 2 * px * cx + cx * cx + py * py - 2 * py * cy + cy * cy +
        pz * pz - 2 * pz * cz + cz * cz - radius * radius;

    // discriminant
    float D = B * B - 4 * A * C;

    float t1 = (-B - sqrt(D)) / (2.0 * A);
    float t2 = (-B + sqrt(D)) / (2.0 * A);

    if (D < 0)
        return false;
    return true;
    //else if ((t1 > 1 || t1 < 0) && (t2 > 1 || t2 < 0))
    //    return false;
    //return true;
}
*/

bool intersectLightSrc(Ray ray, vec3 lightPos, float radius)
{
    vec3 ro = ray.origin - lightPos;
    vec3 rd = ray.direction;
    float b = dot(ro, rd);
    float c = dot(ro, ro) - radius * radius;
    float h = b * b - c;
    if (h < 0.0)
        return false;
    return true;
}

//bool intersectLightSrc(Ray ray, vec3 lightPos, float radius)
//{
//    return dot(ray.direction, normalize(lightPos - ray.origin)) > 0.995;
//}

void main() {
    vec2 pixelSize = vec2(1 / screenResolution.x, 1 / screenResolution.y);
    vec2 pixelOffset = pixelSize * u_SamplePerPixelOffset;
    vec3 viewDir = normalize(vec3((gl_FragCoord.xy * 2 - screenResolution.xy) * tan(radians(u_fov) / 2) / screenResolution.y + pixelOffset, -1.0));
    vec3 worldDir = viewToWorld * viewDir;

    vec3 viewDirPixel = normalize(vec3((gl_FragCoord.xy * 2 - screenResolution.xy) * tan(radians(u_fov) / 2) / screenResolution.y, -1.0));
    vec3 worldDirPixel = viewToWorld * viewDirPixel;

    vec2 uvRes = hash22(fragCoord + 1.0) * screenResolution + screenResolution;
    R_STATE.x = uint(u_seed1.x + uvRes.x);
    R_STATE.y = uint(u_seed1.y + uvRes.x);
    R_STATE.z = uint(u_seed2.x + uvRes.y);
    R_STATE.w = uint(u_seed2.y + uvRes.y);

    int nRays = u_nRaysMax;

    //const float offset = 0.0001;
    //const float offset = 0.001;
    const float offset = 0.01;
    const vec3 baseColor = vec3(1, 1, 1);
    //const vec3 lightPos = vec3(-50, 100, 5);

    float currentLightPower = 1;
    float lightKoeff = 0.7;
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
            if (rayIndex == 0) {
                vec3 skyBoxPos = vec3(worldDirPixel.x, -worldDirPixel.y, worldDirPixel.z);
                resultColor += currentLightPower * vec3(texture(u_skybox, skyBoxPos));
                break;
            }
            vec3 skyBoxPos = vec3(ray.direction.x, -ray.direction.y, ray.direction.z);
            resultColor += currentLightPower * vec3(texture(u_skybox, skyBoxPos));
            break;
        }
        currentPos = hit.position + hit.planeNormal * offset;
        vec3 r = randomOnSphere();
        vec3 dir = normalize(r * dot(r, hit.normal));
        //currentDir = normalize(reflect(ray.direction, hit.normal) * 0 + dir);
        currentDir = reflect(ray.direction, hit.normal);

        
        //Light rays
        //if (u_samplePart < 0.5) {
            for (int lightIndex = 0; lightIndex < u_nPointLights; ++lightIndex) {
                vec3 lightPos = u_pointLightsPos[lightIndex];

                Ray lightRay;
                const float baseRadius = 0.558;
                float radius = baseRadius * u_pointLightsScale[lightIndex];

                vec3 lightPartPos = lightPos + randomOnSphere() * radius;

                lightRay.direction = normalize(lightPartPos - hit.position);
                lightRay.origin = currentPos;
                lightRay.tStart = 0.000001;
                lightRay.tEnd = length(lightPartPos - hit.position);

                float lightDistance = length(lightPos - hit.position);

                Hit lightHit;
                traceCloseHitV2(lightRay, lightHit);

                if (!lightHit.isHit) {
                    float dotProduct = dot(hit.normal, lightRay.direction);
                    //float colorKoeff = (dotProduct + 1.0) / 2.0;
                    float colorKoeff = max(dotProduct, 0.0); // HOLY IT WORKS!
                    float lightSrcPower = 0.4;
                    float koeff = lightSrcPower * colorKoeff;
                    bool test = false;
                    if (test) {
                        if (dotProduct > 0) {
                            float koeff2 = lightSrcPower * ((atan(radius / lightDistance)) / PI) * 10;

                            resultColor += currentLightPower * vec3(1, 1, 1) * koeff2;
                        }
                    }
                    else
                        resultColor += currentLightPower * vec3(1, 1, 1) * koeff;
                }            
                //else {
                //    float dotProduct = dot(hit.normal, lightRay.direction);
                //    float colorKoeff = (dotProduct + 1.0) / 2.0;
                //    float lightSrcPower = 0.4 * colorKoeff;
                //
                //    resultColor += currentLightPower * vec3(1, 1, 1) * lightSrcPower;
                //}
            }
        //}
        
        currentLightPower *= lightKoeff;
    }
    vec3 currColor = resultColor;// / (rayIndex + 1);
    vec3 prevColor = texture(u_previousTexture, fragCoord.xy).xyz;
    color = vec4(mix(prevColor, currColor, u_samplePart), 1);
}
