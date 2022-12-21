#include "GLStereoWidget.h"

#include "qevent.h"
#include "qtimer.h"
#include "qmatrix4x4.h"

static int update_interval = 30;
static float vel_factor = 0.5f;

GLStereoWidget::GLStereoWidget(QWidget *parent, const QGLFormat &format)
	: QGLWidget(format, parent)
	, fullScreen(false)
	, stereo(format.stereo())
{
	setFocusPolicy(Qt::ClickFocus);

	QTimer* timer = new QTimer(this);
	timer->setInterval(update_interval);
	connect(timer, &QTimer::timeout, this, &GLStereoWidget::update);
	timer->start();
}

GLStereoWidget::~GLStereoWidget()
{
}

void GLStereoWidget::initializeGL()
{
	setGeometry(0, 0, 400, 300);
	glClearColor(0.0, 0.0, 0.0, 0);

	stereo = format().stereo();
}

void GLStereoWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLStereoWidget::paintGL()
{
	updateInfo();
	glMatrixMode(GL_MODELVIEW);

	static QColor green = QColor::fromRgb(0, 255, 0);
	static QColor red = QColor::fromRgb(255, 0, 0);

	if (stereo)
	{
		glDrawBuffer(GL_BACK_LEFT);
		drawFrame(green, -0.1f);

		glDrawBuffer(GL_BACK_RIGHT);
		drawFrame(red, 0.1f);
	}
	else
	{
		drawFrame(red, 0.0f);
	}
}

void GLStereoWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_R:
		resetView();
		updateGL();
		break;
	case Qt::Key_Right:
		velocity.setX(qMin(velocity.x(), 0.0f) + 1);
		break;
	case Qt::Key_Left:
		velocity.setX(qMax(velocity.x(), 0.0f) - 1);
		break;
	case Qt::Key_Up:
		velocity.setY(qMin(velocity.y(), 0.0f) + 1);
		break;
	case Qt::Key_Down:
		velocity.setY(qMax(velocity.y(), 0.0f) - 1);
		break;
	}
}

void GLStereoWidget::keyReleaseEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Right:
		velocity.setX(qMax(velocity.x(), 0.0f) - 1);
		break;
	case Qt::Key_Left:
		velocity.setX(qMin(velocity.x(), 0.0f) + 1);
		break;
	case Qt::Key_Up:
		velocity.setY(qMax(velocity.y(), 0.0f) - 1);
		break;
	case Qt::Key_Down:
		velocity.setY(qMin(velocity.y(), 0.0f) + 1);
		break;
	}
}

void GLStereoWidget::setFullScreen(bool _fullScreen)
{
	fullScreen = _fullScreen;
	if (fullScreen)
	{
		setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
		//setWindowFlags(Qt::Window);
		showFullScreen();
	}
	else
	{
		setWindowFlags(Qt::SubWindow);
		showNormal();
	}
}

void GLStereoWidget::setInfo(QString text)
{
	info = text.split("\n");
}

void GLStereoWidget::update()
{
	if (!velocity.isNull())
	{
		pos += velocity * update_interval * vel_factor * 0.001f;
		updateGL();
	}
}

void GLStereoWidget::updateInfo()
{
	QStringList _info;
	_info.append(tr("NativeWindow: %0").arg(window() == this));
	_info.append(tr("FullScreen: %0").arg(fullScreen));

	_info.append(tr("Stereo: %0").arg(stereo));
	_info.append("");

	QWidget* pw = parentWidget();
	QString pwInfo = "null";
	if (pw)
	{
		auto cr = pw->contentsRect();
		pwInfo = tr("(%0, %1, %2, %3)").arg(cr.x()).arg(cr.y()).arg(cr.width()).arg(cr.height());
	}
	_info.append(tr("Parent: %0").arg(pwInfo));

	auto wr = rect();
	QString glInfo = tr("(%0, %1, %2, %3)").arg(wr.x()).arg(wr.y()).arg(wr.width()).arg(wr.height());
	_info.append(tr("GLWidget: %0").arg(glInfo));
	
	_info.append(tr("Camera: %0, %1").arg(pos.x()).arg(pos.y()));
	info = _info;
}

void GLStereoWidget::renderInfo()
{
	int x = 10;
	int y = 20;

	for (int l = 0; l < info.size(); l++)
	{
		renderText(x, y, info.at(l));
		y += 20;
	}
}

void GLStereoWidget::drawFrame(QColor color, float offset)
{
	glClear(GL_COLOR_BUFFER_BIT);

	qglColor(color);

	glPushMatrix();

	QMatrix4x4 modelview;
	modelview.translate(offset, 0.0f, 0.0f);
	modelview.translate(pos);
	glLoadMatrixf(modelview.data());
	drawRectangles();
	glPopMatrix();

	glPolygonMode(GL_BACK, GL_FILL);
	qglColor(QColor::fromRgb(255, 255, 255));
	renderInfo();
}

void GLStereoWidget::drawRectangles()
{
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	// CCW: CounterClockWise
	glFrontFace(GL_CCW);

	float points[] = { -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 1.0f };
	for (int i = 0; i < sizeof(points) / sizeof(points[0]); i += 2)
	{
		glPushMatrix();
		glTranslatef(points[i], points[i + 1], 0.0f);

		// counter clockwise square
		glBegin(GL_POLYGON);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
		glEnd();

		glPopMatrix();
	}
}