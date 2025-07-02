#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Mosquito position
float mosquitoX = 0.0f;
float mosquitoY = 0.6f;

// Tub position and size
float tubWidth = 0.6f, tubHeight = 0.25f;
float tubX = -tubWidth / 2.0f;
float tubY = -0.85f;

// Aerator motor
float aeratorX = tubX + tubWidth/2;
float aeratorY = tubY + tubHeight;
float aeratorSize = 0.15f;
float bladeRotation = 0.0f;
bool aeratorActive = false;
float aeratorTimer = 0.0f;

// Human structure
struct Human {
    float x, y;
    float speed;
    bool direction;
};
std::vector<Human> humans;

// Cloud structure
struct Cloud {
    float x, y;
    float speed;
    float size;
};
std::vector<Cloud> clouds;

// Car structure
struct Car {
    float x, y;
    float speed;
    bool direction; // true = right, false = left
    float r, g, b; // Color components
};
std::vector<Car> cars;

// Larva struct
struct Larva {
    float x, y;
    bool landed;
    bool destroyed;
};
std::vector<Larva> larvae;

void drawCircle(float cx, float cy, float rx, float ry, int segments = 100) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * 3.1415926f * i / segments;
        float x = rx * cosf(theta);
        float y = ry * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void drawMosquito(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(0.5f, 0.5f, 1.0f);

    // Single ellipse body (horizontal orientation)
    glColor3f(0.2f, 0.2f, 0.2f); // Dark gray body
    drawCircle(0.0f, 0.0f, 0.25f, 0.035f); // Flattened ellipse for body

    // White stripes on the body (Aedes style)
    glColor3f(1.0f, 1.0f, 1.0f); // White stripes
    float stripeWidth = 0.02f;
    for (float i = -0.2f; i <= 0.2f; i += 0.07f) {
        glBegin(GL_QUADS);
        glVertex2f(i, 0.035f);
        glVertex2f(i + stripeWidth, 0.035f);
        glVertex2f(i + stripeWidth, -0.035f);
        glVertex2f(i, -0.035f);
        glEnd();
    }

    // Head
    glColor3f(0.1f, 0.1f, 0.1f);
    drawCircle(0.25f, 0.0f, 0.04f, 0.04f);

    // Eyes
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(0.26f, 0.015f, 0.008f, 0.008f);
    drawCircle(0.26f, -0.015f, 0.008f, 0.008f);

    // Proboscis
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.29f, 0.0f);
    glVertex2f(0.35f, 0.0f);
    glEnd();

    // Wings (pointing backward)
    glColor4f(0.7f, 0.7f, 0.9f, 0.6f); // Semi-transparent

    // Right wing
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, 0.02f);
    glVertex2f(-0.15f, 0.1f);
    glVertex2f(-0.25f, 0.15f);
    glVertex2f(-0.35f, 0.1f);
    glVertex2f(-0.25f, 0.02f);
    glEnd();

    // Left wing
    glBegin(GL_POLYGON);
    glVertex2f(0.0f, -0.02f);
    glVertex2f(-0.15f, -0.1f);
    glVertex2f(-0.25f, -0.15f);
    glVertex2f(-0.35f, -0.1f);
    glVertex2f(-0.25f, -0.02f);
    glEnd();

    // Legs (all below body)
    glColor3f(0.1f, 0.1f, 0.1f);
    for (int i = 0; i < 3; i++) {
        float legBaseX = -0.15f + i * 0.15f;
        glBegin(GL_LINES);
        glVertex2f(legBaseX, -0.02f);
        glVertex2f(legBaseX - 0.15f, -0.2f);
        glEnd();
    }

    glPopMatrix();
}

void drawHuman(float x, float y, bool facingRight) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    if (!facingRight) glScalef(-1.0f, 1.0f, 1.0f);

    // Head (more oval shape)
    glColor3f(0.96f, 0.80f, 0.69f);
    drawCircle(0.0f, 0.15f, 0.06f, 0.08f);

    // Neck
    glBegin(GL_POLYGON);
    glVertex2f(-0.02f, 0.07f);
    glVertex2f(0.02f, 0.07f);
    glVertex2f(0.02f, 0.1f);
    glVertex2f(-0.02f, 0.1f);
    glEnd();

    // Body (shirt with sleeves)
    glColor3f(0.2f, 0.5f, 0.8f); // Blue shirt
    glBegin(GL_POLYGON);
    glVertex2f(-0.08f, -0.1f);
    glVertex2f(0.08f, -0.1f);
    glVertex2f(0.08f, 0.07f);
    glVertex2f(-0.08f, 0.07f);
    glEnd();

    // Arms (with sleeves)
    glBegin(GL_POLYGON);
    glVertex2f(-0.08f, 0.02f);
    glVertex2f(-0.12f, -0.05f);
    glVertex2f(-0.08f, -0.1f);
    glVertex2f(-0.04f, -0.05f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.08f, 0.02f);
    glVertex2f(0.12f, -0.05f);
    glVertex2f(0.08f, -0.1f);
    glVertex2f(0.04f, -0.05f);
    glEnd();

    // Pants
    glColor3f(0.1f, 0.1f, 0.4f); // Dark blue pants
    glBegin(GL_POLYGON);
    glVertex2f(-0.08f, -0.1f);
    glVertex2f(0.08f, -0.1f);
    glVertex2f(0.06f, -0.25f);
    glVertex2f(-0.06f, -0.25f);
    glEnd();

    // Legs
    glColor3f(0.96f, 0.80f, 0.69f); // Skin color
    glBegin(GL_POLYGON);
    glVertex2f(-0.06f, -0.25f);
    glVertex2f(-0.03f, -0.25f);
    glVertex2f(-0.03f, -0.4f);
    glVertex2f(-0.06f, -0.4f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.03f, -0.25f);
    glVertex2f(0.06f, -0.25f);
    glVertex2f(0.06f, -0.4f);
    glVertex2f(0.03f, -0.4f);
    glEnd();

    // Shoes
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.07f, -0.4f);
    glVertex2f(-0.02f, -0.4f);
    glVertex2f(-0.02f, -0.42f);
    glVertex2f(-0.07f, -0.42f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.02f, -0.4f);
    glVertex2f(0.07f, -0.4f);
    glVertex2f(0.07f, -0.42f);
    glVertex2f(0.02f, -0.42f);
    glEnd();

    // Hair (Improved)
    glColor3f(0.0f, 0.0f, 0.0f); // Black hair

    // Top part of the hair
    glBegin(GL_POLYGON);
    glVertex2f(-0.07f, 0.2f);  // Left side
    glVertex2f(0.07f, 0.2f);   // Right side
    glVertex2f(0.05f, 0.23f);   // Right top curve
    glVertex2f(-0.05f, 0.223f);  // Left top curve
    glEnd();

    // Flowing down the sides (optional for more hair detail)
    glBegin(GL_POLYGON);
    glVertex2f(-0.05f, 0.23f);  // Left side
    glVertex2f(-0.07f, 0.2f);  // Left ear side
    glVertex2f(-0.06f, 0.15f);   // Left side of neck
    glVertex2f(-0.03f, 0.15f);  // Left back of head
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(0.05f, 0.23f);   // Right side
    glVertex2f(0.07f, 0.2f);   // Right ear side
    glVertex2f(0.06f, 0.15f);    // Right side of neck
    glVertex2f(0.03f, 0.15f);   // Right back of head
    glEnd();

    // Facial features
    glColor3f(0.0f, 0.0f, 0.0f);
    // Eyes
    drawCircle(-0.03f, 0.16f, 0.01f, 0.015f);
    drawCircle(0.03f, 0.16f, 0.01f, 0.015f);
    // Mouth (smile)
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.03f, 0.12f);
    glVertex2f(0.0f, 0.1f);
    glVertex2f(0.03f, 0.12f);
    glEnd();

    glPopMatrix();
}

void drawFlowerPot() {
    float potX = 0.0f;
    float potY = -0.9f;

    // Pot
    glColor3f(0.7f, 0.5f, 0.3f);
    glBegin(GL_POLYGON);
    glVertex2f(potX - 0.12f, potY);
    glVertex2f(potX + 0.12f, potY);
    glVertex2f(potX + 0.1f, potY + 0.2f);
    glVertex2f(potX - 0.1f, potY + 0.2f);
    glEnd();

    // Rim
    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(potX - 0.1f, potY + 0.2f);
    glVertex2f(potX + 0.1f, potY + 0.2f);
    glVertex2f(potX + 0.12f, potY + 0.22f);
    glVertex2f(potX - 0.12f, potY + 0.22f);
    glEnd();

    // Plant
    glColor3f(0.2f, 0.6f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(potX - 0.02f, potY + 0.22f);
    glVertex2f(potX + 0.02f, potY + 0.22f);
    glVertex2f(potX + 0.03f, potY + 0.5f);
    glVertex2f(potX - 0.03f, potY + 0.5f);
    glEnd();

    // Flower
    glColor3f(1.0f, 0.5f, 0.8f);
    for (int i = 0; i < 5; i++) {
        float angle = 2.0f * 3.1415926f * i / 5.0f;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(potX, potY + 0.5f);
        glVertex2f(potX + 0.05f*cos(angle-0.3), potY + 0.5f + 0.05f*sin(angle-0.3));
        glVertex2f(potX + 0.1f*cos(angle), potY + 0.5f + 0.1f*sin(angle));
        glVertex2f(potX + 0.05f*cos(angle+0.3), potY + 0.5f + 0.05f*sin(angle+0.3));
        glEnd();
    }

    glColor3f(1.0f, 0.9f, 0.1f);
    drawCircle(potX, potY + 0.5f, 0.03f, 0.03f);
}

void drawCloud(float x, float y, float size) {
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 3; i++) {
        drawCircle(x + i * 0.15f * size - 0.15f, y, 0.1f * size, 0.07f * size);
    }
    drawCircle(x + 0.1f * size, y + 0.03f * size, 0.12f * size, 0.08f * size);
}

void drawBuilding(float x, float y, float width, float height) {
    glColor3f(0.6f, 0.6f, 0.7f);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    glColor3f(0.9f, 0.9f, 0.5f);
    for (float wy = y + 0.05f; wy < y + height - 0.05f; wy += 0.1f) {
        for (float wx = x + 0.05f; wx < x + width - 0.05f; wx += 0.1f) {
            glBegin(GL_POLYGON);
            glVertex2f(wx, wy);
            glVertex2f(wx + 0.06f, wy);
            glVertex2f(wx + 0.06f, wy + 0.06f);
            glVertex2f(wx, wy + 0.06f);
            glEnd();
        }
    }
}

void drawSun(float x, float y) {
    glColor3f(1.0f, 0.9f, 0.1f);
    drawCircle(x, y, 0.1f, 0.1f);

    glBegin(GL_LINES);
    for (int i = 0; i < 12; i++) {
        float angle = 2.0f * 3.1415926f * i / 12.0f;
        glVertex2f(x, y);
        glVertex2f(x + 0.15f * cos(angle), y + 0.15f * sin(angle));
    }
    glEnd();
}

void drawCar(float x, float y, float r, float g, float b, bool facingRight) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    if (!facingRight) glScalef(-1.0f, 1.0f, 1.0f);

    // Car body
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(-0.1f, -0.05f);
    glVertex2f(0.1f, -0.05f);
    glVertex2f(0.1f, 0.05f);
    glVertex2f(0.08f, 0.07f);
    glVertex2f(-0.08f, 0.07f);
    glEnd();

    // Windows
    glColor3f(0.7f, 0.8f, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.07f, 0.05f);
    glVertex2f(0.07f, 0.05f);
    glVertex2f(0.05f, 0.07f);
    glVertex2f(-0.05f, 0.07f);
    glEnd();

    // Wheels
    glColor3f(0.1f, 0.1f, 0.1f);
    drawCircle(-0.07f, -0.05f, 0.03f, 0.02f);
    drawCircle(0.07f, -0.05f, 0.03f, 0.02f);

    // Headlight
    glColor3f(1.0f, 1.0f, 0.8f);
    drawCircle(0.1f, 0.0f, 0.02f, 0.01f);

    glPopMatrix();
}

void drawRoad() {
    // Road surface
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -0.55f);
    glVertex2f(1.0f, -0.55f);
    glVertex2f(1.0f, -0.45f);
    glVertex2f(-1.0f, -0.45f);
    glEnd();

    // Road markings
    glColor3f(1.0f, 1.0f, 1.0f);
    for (float x = -1.0f; x < 1.0f; x += 0.2f) {
        glBegin(GL_POLYGON);
        glVertex2f(x, -0.5f);
        glVertex2f(x + 0.1f, -0.5f);
        glVertex2f(x + 0.1f, -0.49f);
        glVertex2f(x, -0.49f);
        glEnd();
    }

    // Sidewalks
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -0.45f);
    glVertex2f(1.0f, -0.45f);
    glVertex2f(1.0f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -0.6f);
    glVertex2f(1.0f, -0.6f);
    glVertex2f(1.0f, -0.55f);
    glVertex2f(-1.0f, -0.55f);
    glEnd();
}

void drawAerator() {
    glColor3f(0.3f, 0.3f, 0.3f);
    drawCircle(aeratorX, aeratorY, aeratorSize, aeratorSize * 0.7f);

    if (aeratorActive) {
        glColor3f(1.0f, 0.0f, 0.0f);
    } else {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    drawCircle(aeratorX, aeratorY + aeratorSize * 0.5f, 0.03f, 0.03f);

    glPushMatrix();
    glTranslatef(aeratorX, aeratorY - aeratorSize * 0.3f, 0.0f);
    glRotatef(bladeRotation, 0.0f, 0.0f, 1.0f);

    glColor3f(0.7f, 0.7f, 0.7f);
    for (int i = 0; i < 4; i++) {
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.0f);
        glVertex2f(0.1f, 0.02f);
        glVertex2f(0.1f, -0.02f);
        glEnd();
    }

    glPopMatrix();

    if (aeratorActive) {
        glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
        for (int i = 0; i < 8; i++) {
            float angle = 2.0f * 3.1415926f * i / 8.0f + bladeRotation * 0.1f;
            float dist = 0.1f + fmod(bladeRotation * 0.01f, 0.1f);
            drawCircle(aeratorX + dist * cos(angle),
                      aeratorY - aeratorSize * 0.3f + dist * sin(angle),
                      0.03f, 0.03f);
        }
    }
}

void drawLarvae() {
    for (auto& l : larvae) {
        if (!l.destroyed) {
            glColor3f(1.0f, 0.84f, 0.0f);
            drawCircle(l.x, l.y, 0.025f, 0.015f);
        }
    }
}

void drawTub() {
    glColor4f(0.4f, 0.7f, 0.9f, 0.5f);
    glBegin(GL_POLYGON);
    glVertex2f(tubX, tubY);
    glVertex2f(tubX + tubWidth, tubY);
    glVertex2f(tubX + tubWidth, tubY + tubHeight);
    glVertex2f(tubX, tubY + tubHeight);
    glEnd();
}

void drawBackground() {
    // Sky gradient
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.8f, 1.0f);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glColor3f(0.7f, 0.9f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    // Ground
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.8f, 0.5f);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(-1.0f, 0.0f);
    glEnd();

    drawBuilding(-0.4f, -0.3f, 0.45f, 0.6f);
    drawBuilding(0.1f, -0.25f, 0.35f, 0.55f);
    drawBuilding(0.6f, -0.35f, 0.5f, 0.7f);

    drawSun(-0.8f, 0.8f);
}

void update(int) {
    // Update clouds
    for (auto& c : clouds) {
        c.x += c.speed;
        if (c.x > 1.5f) {
            c.x = -1.5f;
            c.y = 0.6f + (rand() % 10) * 0.03f;
        }
    }



    // Update cars
    for (auto& car : cars) {
        car.x += car.direction ? car.speed : -car.speed;

        // Wrap around when off screen
        if (car.direction && car.x > 1.5f) {
            car.x = -1.5f;
        } else if (!car.direction && car.x < -1.5f) {
            car.x = 1.5f;
        }
    }

    // Occasionally add new cars
    if (rand() % 5000 < 2) {
        bool direction = rand() % 2;
        float r = static_cast<float>(rand()) / RAND_MAX;
        float g = static_cast<float>(rand()) / RAND_MAX;
        float b = static_cast<float>(rand()) / RAND_MAX;

        if (direction) {
            cars.push_back({-1.5f, -0.5f, 0.01f + (rand() % 10) * 0.001f, direction, r, g, b});
        } else {
            cars.push_back({1.5f, -0.5f, 0.01f + (rand() % 10) * 0.001f, direction, r, g, b});
        }
    }

    // Aerator animation
    if (aeratorActive) {
        bladeRotation += 10.0f;
        aeratorTimer -= 1.0f;
        if (aeratorTimer <= 0.0f) aeratorActive = false;
    } else {
        bladeRotation += 2.0f;
    }

    // Update larvae
    for (auto& l : larvae) {
        if (!l.landed && !l.destroyed) {
            if (l.y > tubY + 0.015f) {
                l.y -= 0.01f;
            } else if (l.x >= tubX && l.x <= tubX + tubWidth) {
                l.landed = true;
            } else {
                l.y -= 0.01f;
            }
        }
    }

    // Check for larvae in tub
    bool larvaeInTub = false;
    for (auto& l : larvae) {
        if (l.landed && !l.destroyed &&
            l.x >= tubX && l.x <= tubX + tubWidth &&
            l.y >= tubY && l.y <= tubY + tubHeight) {
            larvaeInTub = true;
            break;
        }
    }

    // Aerator logic
    if (larvaeInTub && !aeratorActive) {
        aeratorActive = true;
        aeratorTimer = 60.0f;

        for (auto& l : larvae) {
            if (l.landed && !l.destroyed &&
                l.x >= tubX && l.x <= tubX + tubWidth &&
                l.y >= tubY && l.y <= tubY + tubHeight) {
                if (rand() % 100 < 30) l.destroyed = true;
            }
        }
    } else if (aeratorActive) {
        for (auto& l : larvae) {
            if (l.landed && !l.destroyed &&
                l.x >= tubX && l.x <= tubX + tubWidth &&
                l.y >= tubY && l.y <= tubY + tubHeight) {
                if (rand() % 100 < 30) l.destroyed = true;
            }
        }
    }

    // Remove destroyed larvae
    larvae.erase(std::remove_if(larvae.begin(), larvae.end(),
        [](const Larva& l) { return l.destroyed; }), larvae.end());

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();

    for (auto& c : clouds) {
        drawCloud(c.x, c.y, c.size);
    }

    drawRoad();

    for (auto& car : cars) {
        drawCar(car.x, car.y, car.r, car.g, car.b, car.direction);
    }

    for (auto& h : humans) {
        drawHuman(h.x, h.y, h.direction);
    }

    drawMosquito(mosquitoX, mosquitoY);
    drawLarvae();
    drawTub();
    drawAerator();
    drawFlowerPot();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'w': mosquitoY += 0.05f; break;
        case 's': mosquitoY -= 0.05f; break;
        case 'a': mosquitoX -= 0.05f; break;
        case 'd': mosquitoX += 0.05f; break;
        case 13: larvae.push_back({mosquitoX + 0.1f, mosquitoY - 0.05f, false, false}); break;
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    switch (key) {
        case GLUT_KEY_UP: mosquitoY += 0.05f; break;
        case GLUT_KEY_DOWN: mosquitoY -= 0.05f; break;
        case GLUT_KEY_LEFT: mosquitoX -= 0.05f; break;
        case GLUT_KEY_RIGHT: mosquitoX += 0.05f; break;
    }
    glutPostRedisplay();
}

void init() {
    srand(time(0));
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    // Initialize clouds
    for (int i = 0; i < 3; i++) {
        clouds.push_back({-1.0f + i * 0.7f, 0.6f + (rand() % 10) * 0.03f,
                        0.002f + (rand() % 10) * 0.0005f, 1.0f + (rand() % 10) * 0.1f});
    }

    // Initialize humans - one on left, one on right
    humans.push_back({-0.7f, -0.5f, 0.005f, true});  // Left human, facing right
    humans.push_back({0.7f, -0.5f, 0.005f, false});   // Right human, facing left

    // Initialize some cars
    cars.push_back({-0.5f, -0.5f, 0.01f, true, 1.0f, 0.0f, 0.0f}); // Red car moving right
    cars.push_back({0.5f, -0.5f, 0.008f, false, 0.0f, 0.0f, 1.0f}); // Blue car moving left
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Mosquito Defense: Aerator System");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
