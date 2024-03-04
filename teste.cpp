#include <iostream>
#include<glew.h>
#include<freeglut.h>
#include "vec3.h"
#include "ray.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct esfera {
    double rEsfera;
    vec3 esfColor;
    vec3 coord;
};

struct hitInfo {
    vec3 normal;
    vec3 hitPoint;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

struct esfera esfera;

vec3 lightPos(0, 5, 0);
vec3 lightIntensity(0.7, 0.7, 0.7);

vec3 bgColor(100, 100, 100);

int wJanela = 800;
auto spec_ratio = 16.0 / 9.0;
int image_HEIGHT = static_cast<int>(wJanela / spec_ratio);
int hJanela = (image_HEIGHT < 1) ? 1 : image_HEIGHT;
int dJanela = 1;
double vfov = 90;

point3 lookfrom = point3(0, 0, dJanela);
point3 lookat = point3(0, 0, 0);
vec3 up(0, 1, 0);

auto focal_length = (lookfrom - lookat).length();

auto theta = glm::radians(vfov);
auto h = tan(theta / 2);

auto viewport_height = 2.0 * h * focal_length;
auto viewport_width = viewport_height * (static_cast<double>(wJanela) / hJanela);

auto w = unit_vector(lookfrom - lookat);
auto u = unit_vector(cross(up, w));
auto v = cross(w, u);

auto camera_center = lookfrom;


auto viewport_u = vec3(viewport_width, 0, 0) * u;
auto viewport_v = vec3(0, viewport_height, 0) * v;

auto Dx = viewport_u / wJanela;
auto Dy = viewport_v / hJanela;

auto viewport_upper_left = camera_center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;

auto pixel00_loc = viewport_upper_left + 0.5 * (Dx + Dy);

GLubyte* PixelBuffer = new GLubyte[wJanela * hJanela * 3];

bool sphere(const ray& r, double ray_tmin, double ray_tmax, hitInfo& rec) {
    vec3 oc = r.origin() - esfera.coord;
    double a = r.direction().length_squared();
    double b = dot(r.direction(), oc);
    double c = oc.length_squared() - esfera.rEsfera * esfera.rEsfera;

    auto discriminant = b * b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    auto root = (-b - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (-b + sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root)
            return false;
    }

    rec.t = root;
    rec.hitPoint = r.at(rec.t);
    //rec.normal = (rec.hitPoint - esfera.coord) ;
    vec3 outward_normal = (rec.hitPoint - esfera.coord) / esfera.rEsfera;
    rec.set_face_normal(r, outward_normal);
    return true;
}

vec3 ray_color(const ray& r) {
    hitInfo rec;
    if (sphere(r, 0, std::numeric_limits<double>::infinity(), rec)) {

        vec3 ambient = 0.1 * lightIntensity;

        vec3 lightDir = unit_vector(lightPos - rec.hitPoint);
        float diff = std::max(0.0, dot(rec.normal, lightDir));
        vec3 diffuse = diff * lightIntensity;

        vec3 viewDir = unit_vector(camera_center - rec.hitPoint);
        vec3 reflectDir = reflect(-lightDir, rec.normal);

        float spec = pow(std::max(dot(viewDir, reflectDir), 0.0), 128);
        vec3 specular = 1.0 * spec * lightIntensity;

        vec3 color = esfera.esfColor * (specular + diffuse + ambient);

        color = vec3(std::min(color.x(), 255.0), std::min(color.y(), 255.0), std::min(color.z(), 255.0));

        return color;
    }

    return bgColor;
}

void makePixel(int x, int y, double r, double g, double b)
{
    int position;

    if (0 <= x && x < hJanela && 0 <= y && y < wJanela) {
        position = (y + x * wJanela) * 3;
        PixelBuffer[position] = static_cast<int>(r);
        PixelBuffer[position + 1] = static_cast<int>(g);
        PixelBuffer[position + 2] = static_cast<int>(b);
    }
}

void raycasting(void)
{
    int r, g, b;
    glClearColor(0.0, 0.8, 0.8, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);



    for (int i = 0; i < hJanela; i++)
    {
        for (int j = 0; j < wJanela; j++)
        {
            auto pixel_center = pixel00_loc + (i * Dy) + (j * Dx);
            auto ray_direction = pixel_center - camera_center;
            ray raio(camera_center, ray_direction);

            vec3 cor = ray_color(raio);
            makePixel(i, j, cor.x(), cor.y(), cor.z());
        }
    }
    glDrawPixels(wJanela, hJanela, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);



    glFlush();
}



void keyboard(unsigned char key, int mousex, int mousey)
{
    if (key == 'q' || key == 'Q') exit(0);
    if (key == 'd') {
        lightPos.e[0] += 1;
        glutPostRedisplay();
    }
    if (key == 'a') {
        lightPos.e[0] -= 1;
        glutPostRedisplay();
    }

    if (key == 'w') {
        lightPos.e[2] += 1;
        glutPostRedisplay();
    }
    if (key == 's') {
        lightPos.e[2] -= 1;
        glutPostRedisplay();
    }
}


int main(int argc, char** argv)
{

    esfera.rEsfera = 2;
    esfera.coord = vec3(0, 0, -(/*dJanela*/ 0.5 + esfera.rEsfera));
    esfera.esfColor = vec3(255, 0, 0);

    // Negotiating window section
    glutInit(&argc, argv);

    // Seting the window properties
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(wJanela, hJanela);
    glutInitWindowPosition(0, 0);

    // Creating the OpenGL window
    glutCreateWindow("Quadrado giratório");

    // Defining the callback functions
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(raycasting);
    // Entering the main event loop
    glutMainLoop();
}

