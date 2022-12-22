#pragma once

#include <QGLWidget>
#include <qtimer.h>
#include <qvector3d.h>

class GLStereoWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLStereoWidget(QWidget *parent, const QGLFormat &format = QGLFormat::defaultFormat());
	~GLStereoWidget();

	void setFullScreen(bool fullScreen);
	void resetView(){ pos = QVector3D(); }

	void setInfo(QString text);

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;

	void updateInfo();
	void renderInfo();

	void drawFrame(QColor color, float offset = 0.0f);
	void drawRectangles();
	void idleUpdate();
protected slots:
	void updatePos();
	void updateFps();
private:
	QStringList info;
	bool fullScreen;
	bool stereo;
	
	QTimer idleTimer;

	QTimer fpsTimer;
	float fps;
	float frameCount;

	QTimer posTimer;
	QVector3D pos;
	QVector3D velocity;
};
