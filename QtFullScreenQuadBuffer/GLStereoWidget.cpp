#include "GLStereoWidget.h"
#include "qevent.h"

GLStereoWidget::GLStereoWidget(QWidget *parent, const QGLFormat &format)
	: QGLWidget(format, parent)
	, fullScreen(false)
{
}

GLStereoWidget::~GLStereoWidget()
{
}

void GLStereoWidget::initializeGL()
{
	setGeometry(0, 0, 400, 300);
	glClearColor(0.5, 0.0, 0.0, 0);
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
	glClear(GL_COLOR_BUFFER_BIT);

	updateInfo();

	qglColor(QColor::fromRgb(255, 255, 255));
	renderInfo();
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

void GLStereoWidget::updateInfo()
{
	QStringList _info;
	_info.append(tr("NativeWindow: %0").arg(window() == this));
	_info.append(tr("FullScreen: %0").arg(fullScreen));

	bool stereo = format().stereo();
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
