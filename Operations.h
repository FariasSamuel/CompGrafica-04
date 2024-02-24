#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <iostream>
#include<math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 product(glm::mat4 t, glm::vec4 e);
glm::mat4 product(glm::mat4 t, glm::mat4 e);
glm::mat4 translate(glm::mat4 t, glm::vec4 e);
glm::mat4 rotate(glm::mat4 t, float angle, glm::vec3 e);
glm::mat4 pespective(float vf, float ratio, float near, float far);
glm::mat4 lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 direction);
#endif