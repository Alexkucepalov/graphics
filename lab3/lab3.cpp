#include <stdlib.h>
#include <glut.h>
//#include <Windows.h>
#include <gl/GL.h>
#include <iostream>
#include <math.h>



float deltaAngle = 0.0f;
float yAngle = 0.0f;
int xOrigin = -1;
int yOrigin = 0;

double rotate_y = 0;
double rotate_x = 0;
const float PI = 3.14159265f;
const float RADIUS = 1.5f;
const int SLICES = 36;
const int STACKS = 18;


// переменные для углов поворота камеры
float angle = 0.0;
float nAngle = 0.0;
// вектор направления камеры
float lx = 0.0f, lz = -1.0f, ly = 0.0f;
// координаты позиции камеры
float x = 0.0f, z = 7.0f, y = 1.0f;

void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_UP: // при нажатии на стрелку вверх изменеям координаты
		rotate_x += 5;
		break;
	case GLUT_KEY_DOWN:
		rotate_x -= 5;
		break;
	case GLUT_KEY_LEFT:
		rotate_y -= 5;
		break;
	case GLUT_KEY_RIGHT:
		rotate_y += 5;
		break;
	}
	//  Запрос обновления экрана
	glutPostRedisplay();
}

//обрабатываем вращение камеры при нажатии ЛКМ
void mouseButton(int button, int state, int x, int y) {

	// начинаем движение камеры только при нажатии на ЛКМ
	if (button == GLUT_LEFT_BUTTON) {

		// если кнопка зажата
		if (state == GLUT_UP) {
			angle += deltaAngle;
			nAngle += yAngle;

		}
		else
		{
			xOrigin = x;
			yOrigin = y;
		}
	}
}

void mouseMove(int x, int y) {

	// принимает значение True только при нажатии на ЛКМ
	if (xOrigin >= 0) {

		// изменяем значения переменных на которые изменяется угол
		deltaAngle = (x - xOrigin) * 0.001f;
		yAngle = (y - yOrigin) * 0.001f;

		// обновляем направление взгляда камеры
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
		ly = -sin(nAngle + yAngle);
	}
}

void settings()
{
	glClearColor(1, 1, 1, 0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	/* устанавливаем параметры источника света */
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); // фоновый свет
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); // рассеяный свет
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); // отражённый свет
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // позиция источника света

	/* включаем освещение и источник света */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}



void CreateTorus()
{
	/* параметры материала тора */
	float mat1_dif[] = { 0.8f,0.8f,0.0f };
	float mat1_amb[] = { 0.2f,0.2f,0.2f };
	float mat1_spec[] = { 0.6f,0.6f,0.6f };
	float mat1_shininess = 0.5f * 128;

	/* отображаем тор */
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat1_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat1_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat1_spec);
	glMaterialf(GL_FRONT, GL_SHININESS, mat1_shininess);

	glPushMatrix();
	glTranslatef(-0.75, 1.0, 0.0);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glutSolidTorus(0.275, 0.85, 45, 45);
	glPopMatrix();

}

void CreateCone()
{
	float mat2_dif[] = { 0.0f,0.0f,0.8f };
	float mat2_amb[] = { 0.2f,0.2f,0.2f };
	float mat2_spec[] = { 0.6f,0.6f,0.6f };
	float mat2_shininess = 0.7f * 128;

	/* отображаем конус */
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat2_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat2_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat2_spec);
	glMaterialf(GL_FRONT, GL_SHININESS, mat2_shininess);

	glPushMatrix();
	glTranslatef(-3.5, 0.0, 0.0);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	glutSolidCone(1.0, 2.0, 30, 30);


	/* отображаем конус */
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(gluNewQuadric(), 0.0f, 1.0, 30, 1);
	gluDeleteQuadric(gluNewQuadric());
	glPopMatrix();
}

void CreateСylinder()
{
	/* параметры материала цилиндра */
	float mat3_dif[] = { 0.9f,0.2f,0.0f };
	float mat3_amb[] = { 0.2f,0.2f,0.2f };
	float mat3_spec[] = { 0.6f,0.6f,0.6f };
	float mat3_shininess = 0.1f * 128;

	/* отображаем цилиндра */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat3_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat3_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat3_spec);
	glMaterialf(GL_FRONT, GL_SHININESS, mat3_shininess);
	glEnable(GL_CULL_FACE);
	glPushMatrix();
	glTranslatef(2, 0.0, -1.0);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluCylinder(gluNewQuadric(), 1.0, 1.0, 2, 30, 1);


	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 2);
	gluDisk(gluNewQuadric(), 0.0f, 1.0, 30, 1);
	glPopMatrix();
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	gluDisk(gluNewQuadric(), 0.0f, 1.0, 30, 1);
	gluDeleteQuadric(gluNewQuadric());

	glPopMatrix();
	glPopMatrix();
}




void Display(void)
{
	// Очищаем буфер цвета и глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Устанавливаем позицию камеры
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 2.0f, 0.0f);
	glRotatef(rotate_x, 1.0, 0.0, 0.0);
	glRotatef(rotate_y, 0.0, 1.0, 0.0);

	CreateCone();
	CreateСylinder();
	CreateTorus();

	glutSwapBuffers();
}

//подстраиваемся под изменения окна
void Reshape(int x, int y)
{
	//если изменений не было то выходим из функции
	if (y == 0 || x == 0) return;

	//устанавливаем новую проекцию матрицы
	glMatrixMode(GL_PROJECTION);
	//сбрасываем матрицу в начальное состояние с помощью матрицы идентичности
	glLoadIdentity();
	//устанавливаем угол обзора 50, минимальную отрисовку = 0.5 максимальную = 35
	gluPerspective(50.0, (GLdouble)x / (GLdouble)y, 0.5f, 35.0f);


	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, x, y);
}

void Idle()
{
	Display();
}



int main(int argc, char** argv)
{
	//Задаём начальные параметры GLUT
	glutInit(&argc, argv);//инициализация GLUT
	glutInitDisplayMode(GLUT_DOUBLE);//режим отображения
	glutInitWindowSize(1380, 720);//размер окна программы

	//Создаём окно программы с заданным названием
	glutCreateWindow("OpenGL: тор, конус, цилииндр");

	settings();



	glutDisplayFunc(Display);//событие отображения объектов
	glutReshapeFunc(Reshape);// событие изменение окна программы
	glutIdleFunc(Idle); // событие перерисовки кадров
	glutSpecialFunc(processSpecialKeys); //событие обработки нажатия специальных клавиш



	//функции для обработки событий мыши
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutMainLoop();

	return 0;
}

