#pragma once

#include <QGLWidget>
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
protected slots:
	void update();
private:
	QStringList info;
	bool fullScreen;
	bool stereo;

	QVector3D pos;
	QVector3D velocity;
};
