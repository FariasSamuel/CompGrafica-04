#include "Operations.h"

glm::mat4 product(glm::mat4 t, glm::vec4 e) {
    glm::mat4 m(1.0f);
    for (int i = 0; i < 4; i++) {
        m[i][i] = e[i];
    }
    glm::mat4 v(0.0f);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                //std::cout << t[i][k] << " * " << t[i][k] << std::endl;
                v[i][j] += t[i][k] * m[k][j];
            }
        }
    }
    return v;
}

glm::mat4 product(glm::mat4 t, glm::mat4 e) {
    glm::mat4 v(0.0f);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                //std::cout << t[i][k] << " * " << t[i][k] << std::endl;
                v[i][j] += t[i][k] * e[k][j];
            }
        }
    }
    return v;
}

glm::mat4 translate(glm::mat4 t, glm::vec4 e) {
    glm::mat4 m(1.0f);

    for (int i = 0; i < 3; i++) {
        m[i][4] = m[i][4] + e[i];
    }
    return m;
}

glm::mat4 rotate(glm::mat4 t, float angle, glm::vec3 e) {

    glm::mat4 m(1.0f);
    if (e.z != 0) {
        m[0][0] = cos(angle);
        m[0][1] = -1 * sin(angle);
        m[1][0] = sin(angle);
        m[1][1] = cos(angle);
    }
    if (e.x != 0) {
        m[1][1] = cos(angle);
        m[1][2] = -1 * sin(angle);
        m[2][1] = sin(angle);
        m[2][2] = cos(angle);
    }
    if (e.y != 0) {
        m[0][0] = cos(angle);
        m[0][2] = -1 * sin(angle);
        m[2][0] = sin(angle);
        m[2][2] = cos(angle);

    }
    glm::mat4 v(0.0f);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                //std::cout << t[i][k] << " * " << t[i][k] << std::endl;
                v[i][j] += t[i][k] * m[k][j];
            }
        }
    }
    return v;
}

glm::mat4 pespective(float vf, float ratio, float near, float far) {

    glm::mat4 m(0.0f);
    vf = 1 / tan(vf * 0.5);
    m[0][0] = vf / ratio;
    m[1][1] = vf;
    m[2][2] = (far + near) / (near - far);
    m[3][2] = (2 * far * near) / (near - far);
    m[2][3] = -1;

    return m;
}

glm::mat4 lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 direction) {

    glm::mat4 m(0.0f);

    glm::vec3 cameraDirection = glm::normalize(position - target);


    glm::vec3 cameraRight = glm::normalize(glm::cross(direction, cameraDirection));

    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    for (int i = 0; i < 3; i++) {
        m[0][i] = cameraRight[i];
    }
    for (int i = 0; i < 3; i++) {
        m[1][i] = cameraUp[i];
    }
    for (int i = 0; i < 3; i++) {
        m[2][i] = cameraDirection[i];
    }

    m[3][3] = 1;


    glm::mat4 p(1.0f);

    for (int i = 0; i < 3; i++) {
        p[i][3] = -1 * position[i];
        //std::cout << position[i] << std::endl;
    }
    m = product(m, p);
    glm::mat4 t(0.0f);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            t[i][j] = m[j][i];
        }
    }

    return t;
}